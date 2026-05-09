#include <aether/objects/Node.hh>
#include <aether/systems/Window.hh>
#include <aether/systems/Renderer.hh>
#include <aether/math/util.hh>
#include <aether/common/log.hh>
#include <fmt/format.h>
#include <algorithm>
#include <cmath>

// TODO: world alpha depending on parent alpha

namespace ae {

Node::Node() :
	color_(255, 255, 255),
	transform_(mat3::identity()),
	bounds_(0.f, 0.f),
	position_(0.f, 0.f),
	anchor_(0.5f, 0.5f),
	scale_(1.f, 1.f),
	skew_(0.f, 0.f),
	rotation_(0.f),
	alpha_(1.f),
	combined_alpha_(1.f),
	time_scale_(1.f),
	is_transform_dirty_(false),
	is_alpha_dirty_(false),
	is_active_(false),
	is_visible_(true),
	is_initialized_(false)
{}

Node::~Node() = default;

void Node::add(std::shared_ptr<Node> node) {
	if (!node) {
		return;
	}

	auto self = shared_from_this();

	if (node == self) {
		// prevent self-parenting
		return;
	}

	if (node->has_ancestor(self)) {
		// prevent hierarchy cycle
		return;
	}

	if (std::find(children_.begin(), children_.end(), node) != children_.end()) {
		// prevent duplicates
		return;
	}

	if (auto old_parent = node->parent().lock()) {
		// remove from old parent
		old_parent->remove(node);
	}

	node->parent_ = weak_from_this();
	children_.emplace_back(node);
	node->mark_transform_dirty();
	node->mark_alpha_dirty();
}

void Node::remove(std::shared_ptr<Node> node) {
	if (!node) {
		return;
	}

	if (node == shared_from_this()) {
		// prevent self-remove
		return;
	}

	auto it = std::find(children_.begin(), children_.end(), node);
	if (it == children_.end()) {
		return;
	}

	node->parent_.reset();
	children_.erase(it);
	node->mark_transform_dirty();
	node->mark_alpha_dirty();
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

void Node::activate() {
	is_active_ = true;
}

void Node::deactivate() {
	is_active_ = false;
}

void Node::toggle_active(bool val) {
	is_active_ = val;
}

bool Node::is_active() const {
	return is_active_;
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

std::weak_ptr<Node> Node::parent() const {
	return parent_;
}

void Node::set_name(std::string_view name) {
	name_ = std::string(name);
}

std::string_view Node::name() const {
	return name_;
}

std::string_view Node::type() const {
	return "Node";
}

void Node::set_bounds(size<float> val) {
	bounds_ = val;
	mark_transform_dirty();
}

size<float> Node::bounds() const {
	return bounds_;
}

void Node::set_position(vec2<float> val) {
	position_ = val;
	mark_transform_dirty();
}

vec2<float> Node::position() const {
	return position_;
}

void Node::set_anchor(vec2<float> val) {
	anchor_ = val;
	mark_transform_dirty();
}

vec2<float> Node::anchor() const {
	return anchor_;
}

void Node::set_scale(vec2<float> val) {
	scale_ = val;
	mark_transform_dirty();
}

void Node::set_scale(float val) {
	scale_ = {
		.x = val,
		.y = val
	};

	mark_transform_dirty();
}

vec2<float> Node::scale() const {
	return scale_;
}

void Node::set_skew(vec2<float> val) {
	skew_ = val;
	mark_transform_dirty();
}

vec2<float> Node::skew() const {
	return skew_;
}

void Node::set_rotation(float val) {
	rotation_ = val;
	mark_transform_dirty();
}

float Node::rotation() const {
	return rotation_;
}

void Node::set_color(rgb val) {
	color_ = val;
}

rgb Node::color() const {
	return color_;
}

void Node::set_alpha(float val) {
	alpha_ = std::clamp(val, 0.f, 1.f);
	mark_alpha_dirty();
}

float Node::alpha() const {
	return alpha_;
}

void Node::toggle_visibility(bool val) {
	is_visible_ = val;
}

bool Node::is_visible() const {
	return is_visible_;
}

void Node::set_time_scale(float val) {
	time_scale_ = std::max(0.f, val);
}

float Node::time_scale() const {
	return time_scale_;
}

// protected
bool Node::init(Context const& ctx) {
	return true;
}

// protected
void Node::update(Context const& ctx, float dt) {}

// protected
void Node::draw(Context const& ctx, mat3 const& transform, float alpha) const {}

// private
bool Node::base_init(Context const& ctx) {
	if (is_initialized_) {
		return true;
	}

	return is_initialized_ = init(ctx);
}

// private
void Node::base_update(Context const& ctx, float dt) {
	if (!is_initialized_) {
		return;
	}

	float world_dt = dt * time_scale_;

	if (is_active_) {
		update(ctx, world_dt);
	}

	for (auto const& node : children_) {
		if (!node) {
			continue;
		}

		node->base_update(ctx, world_dt);
	}
}

// private
void Node::base_draw(Context const& ctx) {
	if (!is_initialized_ || !is_visible_) {
		return;
	}

	if (is_alpha_dirty_) {
		combined_alpha_ = calculate_combined_alpha(parent_);
		is_alpha_dirty_ = false;
	}

	if (combined_alpha_ == 0.f) {
		return;
	}

	if (is_transform_dirty_) {
		transform_ = calculate_transform(ctx, parent_);
		is_transform_dirty_ = false;
	}

	draw(ctx, transform_, combined_alpha_);

	for (auto const& node : children_) {
		if (!node) {
			continue;
		}

		node->base_draw(ctx);
	}
}

// private
bool Node::has_ancestor(std::shared_ptr<Node> node) const {
	auto p = parent().lock();

	while (p) {
		if (p == node) {
			return true;
		}

		p = p->parent().lock();
	}

	return false;
}

// private
void Node::mark_transform_dirty() {
	if (is_transform_dirty_) {
		// already dirty
		return;
	}

	is_transform_dirty_ = true;

	for (auto& child : children_) {
		if (child) {
			child->mark_transform_dirty();
		}
	}
}

// private
void Node::mark_alpha_dirty() {
	if (is_alpha_dirty_) {
		// already dirty
		return;
	}

	is_alpha_dirty_ = true;

	for (auto& child : children_) {
		if (child) {
			child->mark_alpha_dirty();
		}
	}
}

// private
mat3 Node::calculate_transform(Context const& ctx, std::weak_ptr<Node> parent) const {
	vec2<float> anchor_position = {
		.x = anchor_.x * bounds_.width,
		.y = anchor_.y * bounds_.height
	};

	vec2<float> skew_rad = {
		.x = math::degrees_to_radians(skew_.x),
		.y = math::degrees_to_radians(skew_.y)
	};

	mat3 t = mat3::translation(position_);
	mat3 r = mat3::rotation(math::degrees_to_radians(rotation_));
	mat3 s = mat3::scale(scale_);
	mat3 k = mat3::skew(skew_rad);
	mat3 a = mat3::translation(-anchor_position);
	mat3 local = t * r * s * k * a;

	if (auto p = parent.lock()) {
		return p->transform_ * local;
	}

	return local;
}

// private
float Node::calculate_combined_alpha(std::weak_ptr<Node> parent) const {
	if (auto p = parent.lock()) {
		return std::clamp(alpha_ * p->alpha(), 0.f, 1.f);
	}

	return alpha_;
}

}