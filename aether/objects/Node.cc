#include <aether/objects/Node.hh>
#include <aether/systems/Window.hh>
#include <aether/math/util.hh>
#include <algorithm>
#include <cmath>

namespace ae {

Node::Node() :
	color(255, 255, 255),
	pos(0.f, 0.f),
	anchor(0.5f, 0.5f),
	scale(1.f, 1.f),
	skew(0.f, 0.f),
	rotation(0.f),
	alpha(1.f),
	time_scale(1.f),
	is_visible(true),
	is_active(true),
	bounds_(0.f, 0.f),
	local_transform_(mat3::identity()),
	world_transform_(mat3::identity()),
	world_alpha_(1.f),
	last_rotation_(-1.f),
	last_alpha_(-1.f),
	last_bounds_(-1.f, -1.f),
	last_pos_(-1.f, -1.f),
	last_anchor_(-1.f, -1.f),
	last_scale_(-1.f, -1.f),
	last_skew_(-1.f, -1.f),
	is_dirty_(false),
	is_initialized_(false)
{}

Node::~Node() = default;

void Node::add(sptr<Node> vessel) {
	if (!vessel) {
		return;
	}

	auto self = shared_from_this();

	if (vessel == self) {
		// prevent self-parenting
		return;
	}

	if (vessel->has_ancestor(self)) {
		// prevent hierarchy cycle
		return;
	}

	if (std::find(children_.begin(), children_.end(), vessel) != children_.end()) {
		// prevent duplicates
		return;
	}

	if (auto old_parent = vessel->parent().lock()) {
		// remove from old parent
		old_parent->remove(vessel);
	}

	vessel->parent_ = weak_from_this();
	children_.emplace_back(vessel);
}

void Node::remove(sptr<Node> vessel) {
	if (!vessel) {
		return;
	}

	auto self = shared_from_this();

	if (vessel == self) {
		// prevent self-remove
		return;
	}

	auto it = std::find(children_.begin(), children_.end(), vessel);
	if (it == children_.end()) {
		return;
	}

	vessel->parent_.reset();
	children_.erase(it);
}

void Node::destroy() {
	if (!is_initialized_) {
		return;
	}

	is_initialized_ = false;

	if (auto parent = parent_.lock()) {
		parent->remove(shared_from_this());
	}

	// recursively destroy children
	while (!children_.empty()) {
		auto child = children_.back();
		children_.pop_back();
		if (child) {
			child->parent_.reset();
			child->destroy();
		}
	}
}

size_t Node::count() const {
	size_t c = children_.size();

	for (auto const& child : children_) {
		if (!child) {
			continue;
		}

		c += child->count();
	}

	return c;
}

float Node::world_rotation() const {
	return std::atan2(world_transform_.m[1][0], world_transform_.m[0][0]);
}

vec2<float> Node::world_position() const {
	return world_transform_.translation();
}

vec2<float> Node::world_scale() const {
	float sx = std::sqrt(
		world_transform_.m[0][0] * world_transform_.m[0][0] +
		world_transform_.m[0][1] * world_transform_.m[0][1]
	);

	float sy = std::sqrt(
		world_transform_.m[1][0] * world_transform_.m[1][0] +
		world_transform_.m[1][1] * world_transform_.m[1][1]
	);

	return vec2<float>(sx, sy);
}

size<float> Node::world_size() const {
	return size<float>(
		bounds_.width * world_scale().x,
		bounds_.height * world_scale().y
	);
}

float Node::world_alpha() const {
	return world_alpha_;
}

size<float> Node::bounds() const {
	return bounds_;
}

wptr<Node> Node::parent() const {
	return parent_;
}

void Node::set_name(std::string_view name) {
	name_ = std::string(name);
}

std::string_view Node::name() const {
	return name_;
}

std::string_view Node::type() const {
	static constexpr std::string_view TYPE = "Node";
	return TYPE;
}

// protected
bool Node::init(Context const& ctx) {
	return true;
}

// protected
void Node::update(Context const& ctx, float dt) {}

// protected
void Node::draw(Context const& ctx) const {}

// protected
mat3 Node::world_transform() const {
	return world_transform_;
}

// private
bool Node::base_init(Context const& ctx) {
	if (is_initialized_) {
		return true;
	}

	if (!init(ctx)) {
		return false;
	}

	is_initialized_ = true;
	return true;
}

// private
void Node::base_update(Context const& ctx, float dt) {
	if (!is_initialized_ || !is_active) {
		return;
	}

	alpha = std::clamp(alpha, 0.f, 1.f);
	bool window_was_resized = ctx.window() ? ctx.window()->is_resized() : false;

	if (
		window_was_resized ||
		last_pos_ != pos ||
		last_alpha_ != alpha ||
		last_anchor_ != anchor ||
		last_rotation_ != rotation ||
		last_scale_ != scale ||
		last_bounds_ != bounds_ ||
		last_skew_ != skew
	) {
		last_pos_ = pos;
		last_alpha_ = alpha;
		last_anchor_ = anchor;
		last_rotation_ = rotation;
		last_scale_ = scale;
		last_bounds_ = bounds_;
		last_skew_ = skew;
		mark_dirty();
	}

	if (is_dirty_) {
		on_dirty(ctx);
		is_dirty_ = false;
	}

	float world_dt = dt * time_scale;
	update(ctx, world_dt);

	for (auto const& vessel : children_) {
		if (!vessel) {
			continue;
		}

		vessel->base_update(ctx, world_dt);
	}
}

// private
void Node::base_draw(Context const& ctx) const {
	if (
		!is_initialized_ ||
		!is_visible ||
		world_alpha_ == 0.f
	) {
		return;
	}

	draw(ctx);

	for (auto const& v : children_) {
		if (!v) {
			continue;
		}

		v->base_draw(ctx);
	}
}

// private
bool Node::has_ancestor(sptr<Node> const& vessel) const {
	auto p = parent().lock();

	while (p) {
		if (p == vessel) {
			return true;
		}

		p = p->parent().lock();
	}

	return false;
}

// private
void Node::mark_dirty() {
	if (!is_dirty_) {
		is_dirty_ = true;

		for (auto& child : children_) {
			if (child) {
				child->mark_dirty();
			}
		}
	}
}

void Node::on_dirty(Context const& ctx) {
	auto anchor_offset = vec2<float>(
		anchor.x * bounds_.width,
		anchor.y * bounds_.height
	);

	auto skew_rad = vec2<float>(
		math::degrees_to_radians(skew.x),
		math::degrees_to_radians(skew.y)
	);

	float rotation_rad = math::degrees_to_radians(rotation);

	mat3 T = mat3::translation(pos);
	mat3 R = mat3::rotation(rotation_rad);
	mat3 S = mat3::scale(scale);
	mat3 K = mat3::skew(skew_rad);
	mat3 A = mat3::translation(-anchor_offset);

	local_transform_ = T * R * S * K * A;

	if (auto p = parent_.lock()) {
		world_transform_ = p->world_transform_ * local_transform_;
		world_alpha_ = std::clamp(alpha * p->world_alpha_, 0.f, 1.f);
	} else {
		float dpi_scale = ctx.dpi_scale();
		mat3 UI = mat3::scale(vec2<float>(dpi_scale, dpi_scale));
		world_transform_ = UI * local_transform_;
		world_alpha_ = alpha;
	}
}

}