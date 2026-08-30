#include <aether/math.hh>
#include <aether/node/components/transform.hh>
#include <aether/node/node.hh>

namespace aether {

transform::transform(context const& ctx, strong_ref<node> n) noexcept
        : node_component(ctx, n)
        , matrix_(mat3::identity())
        , anchor_(0.5f)
        , scale_(1.f)
        , shear_(1.f)
        , is_flipped_(false)
        , rotation_(0.f)
        , is_transform_dirty_(false) {
}

void transform::set_bounds(size<uint32_t> val) noexcept {
	if (bounds_ == val) {
		return;
	}
	bounds_ = val;
	mark_transform_dirty_();
}

void transform::set_position(vec2<float> val) noexcept {
	if (position_ == val) {
		return;
	}
	position_ = val;
	mark_transform_dirty_();
}

void transform::set_anchor(vec2<float> val) noexcept {
	val = clamp(val, vec2<float>(0.f), vec2<float>(1.f));
	if (anchor_ == val) {
		return;
	}
	anchor_ = val;
	mark_transform_dirty_();
}

void transform::set_scale(vec2<float> val) noexcept {
	val = max(val, vec2<float>(0.f));
	if (scale_ == val) {
		return;
	}
	scale_ = val;
	mark_transform_dirty_();
}

void transform::set_shear(vec2<float> val) noexcept {
	if (shear_ == val) {
		return;
	}
	shear_ = val;
	mark_transform_dirty_();
}

mat3 transform::matrix() const noexcept {
	return matrix_;
}

size<uint32_t> transform::bounds() const noexcept {
	return bounds_;
}

vec2<float> transform::position() const noexcept {
	return position_;
}

vec2<float> transform::anchor() const noexcept {
	return anchor_;
}

vec2<float> transform::scale() const noexcept {
	return scale_;
}

vec2<float> transform::shear() const noexcept {
	return shear_;
}

bool transform::init_() noexcept {
	if (!node_component::init_()) {
		return false;
	}
	this->schedule(schedule_level::visit);
	return true;
}

void transform::visit_() noexcept {
	node_component::visit_();
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

	is_transform_dirty_          = true;
	strong_ref<node> strong_node = this->weak_node_.construct();

	if (!strong_node) {
		return;
	}

	for (strong_ref<node>& child : strong_node->children()) {
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

	matrix_                      = t * r * s * k * a;
	strong_ref<node> strong_node = this->weak_node_.construct();

	if (!strong_node) {
		return;
	}

	strong_ref<node> node_parent = strong_node->parent().construct();

	if (!node_parent) {
		return;
	}

	if (transform* t = node_parent->component<transform>()) {
		t->matrix_ *= matrix_;
	}
}

} // namespace aether