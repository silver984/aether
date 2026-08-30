#include <aether/math.hh>
#include <aether/node/components/transform.hh>
#include <aether/node/components/visibility.hh>
#include <aether/node/node.hh>

namespace aether {

void transform::set_bounds(size<uint32_t> val) {
	if (bounds_ == val) {
		return;
	}
	bounds_ = val;
	mark_transform_dirty_();
}

void transform::set_position(vec2<float> val) {
	if (position_ == val) {
		return;
	}
	position_ = val;
	mark_transform_dirty_();
}

void transform::set_anchor(vec2<float> val) {
	val = clamp(val, vec2<float>(0.f), vec2<float>(1.f));
	if (anchor_ == val) {
		return;
	}
	anchor_ = val;
	mark_transform_dirty_();
}

void transform::set_scale(vec2<float> val) {
	val = max(val, vec2<float>(0.f));
	if (scale_ == val) {
		return;
	}
	scale_ = val;
	mark_transform_dirty_();
}

void transform::set_shear(vec2<float> val) {
	if (shear_ == val) {
		return;
	}
	shear_ = val;
	mark_transform_dirty_();
}

void transform::set_flipped(vec2<bool> val) {
	if (is_flipped_ == val) {
		return;
	}
	is_flipped_ = val;
	mark_transform_dirty_();
}

void transform::set_rotation(float val) {
	if (rotation_ == val) {
		return;
	}
	rotation_ = val;
	mark_transform_dirty_();
}

void transform::node_parented_() {
	node_component::node_parented_();
	mark_transform_dirty_();
}

void transform::node_detached_() {
	node_component::node_detached_();
	mark_transform_dirty_();
}

void transform::visit_() {
	node_component::visit_();
	visibility* v = this->strong_node_()->component<visibility>();
	if (v && !v->is_visible()) {
		return;
	}
	if (is_transform_dirty_) {
		is_transform_dirty_ = false;
		update_matrix_();
	}
}

void transform::mark_transform_dirty_() {
	if (is_transform_dirty_) {
		// already dirty
		return;
	}
	is_transform_dirty_ = true;
	for (auto& child : this->strong_node_()->children()) {
		if (transform* t = child->component<transform>()) {
			t->mark_transform_dirty_();
		}
	}
}

void transform::update_matrix_() {
	// todo: use scene camera, and also scroll factor

	vec2<float> const anchor_position = {
	        anchor_.x * bounds_.width,
	        anchor_.y * bounds_.height,
	};

	vec2<float> const shear_rad = {
	        degrees_to_radians(shear_.x),
	        degrees_to_radians(shear_.y),
	};

	vec2<float> const scale_factor = {
	        is_flipped_.x ? -1.f : 1.f,
	        is_flipped_.y ? -1.f : 1.f,
	};

	mat3 const t = mat3::translation(position_);
	mat3 const r = mat3::rotation(degrees_to_radians(rotation_));
	mat3 const s = mat3::scale(scale_ * scale_factor);
	mat3 const k = mat3::skew(shear_rad);
	mat3 const a = mat3::translation(-anchor_position);

	matrix_            = t * r * s * k * a;
	strong_ref<node> p = this->strong_node_()->parent().construct();

	if (!p) {
		return;
	}

	if (transform* t = p->component<transform>()) {
		matrix_ *= t->matrix_;
	}
}

} // namespace aether