#include <algorithm>
#include <cmath>
#include <objects/Node.hh>
#include <util/math.hh>

namespace aether {

Node::Node(Context const& ctx)
    : ctx_(ctx)
    , color_(255)
    , combined_color_(color_)
    , transform_(mat3::identity())
    , anchor_(0.5f, 0.5f)
    , scale_(1.f, 1.f)
    , rotation_(0.f)
    , time_scale_(1.f)
    , is_flip_x_(false)
    , is_flip_y_(false)
    , is_transform_dirty_(false)
    , is_rgba_dirty_(false)
    , is_active_(false)
    , is_draw_scheduled_(false)
    , is_visible_(true) {}

Node::~Node() = default;

void Node::add_child(std::shared_ptr<Node> node) {
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
		old_parent->remove_child(node);
	}

	node->parent_ = weak_from_this();
	children_.emplace_back(node);
	node->mark_transform_dirty();
	node->mark_rgba_dirty();
}

void Node::remove_child(std::shared_ptr<Node> node) {
	if (!node) {
		return;
	}

	if (node == shared_from_this()) {
		// prevent self-remove
		return;
	}

	auto const iterator = std::find(children_.begin(), children_.end(), node);

	if (iterator == children_.end()) {
		return;
	}

	node->parent_.reset();
	children_.erase(iterator);
	node->mark_transform_dirty();
	node->mark_rgba_dirty();
}

void Node::destroy() {
	if (auto parent = parent_.lock()) {
		parent->remove_child(shared_from_this());
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

std::shared_ptr<Node> Node::fetch_child(std::string_view name) {
	auto const iterator = std::find_if(children_.begin(), children_.end(), [&](std::shared_ptr<Node> const& child) {
		return child && child->name() == name;
	});

	return (iterator != children_.end()) ? *iterator : nullptr;
}

void Node::activate() {
	is_active_ = true;
}

void Node::deactivate() {
	is_active_ = false;
}

void Node::schedule_draw() {
	is_draw_scheduled_ = true;
}

void Node::unschedule_draw() {
	is_draw_scheduled_ = false;
}

size_t Node::child_count() const {
	return children_.size();
}

size_t Node::recursed_child_count() const {
	size_t c = children_.size();

	for (auto const& child : children_) {
		if (!child) {
			continue;
		}

		c += child->recursed_child_count();
	}

	return c;
}

std::weak_ptr<Node> Node::parent() const {
	return parent_;
}

void Node::set_name(std::string_view name) {
	if (name_ == name) {
		return;
	}

	name_ = std::string(name);
}

std::string_view Node::name() const {
	return name_;
}

std::string_view Node::type() const {
	return "Node";
}

void Node::set_bounds(size<int> val) {
	val                          = util::max({}, val);
	size<uint32_t> const valui32 = static_cast<size<uint32_t>>(val);

	if (bounds_ == valui32) {
		return;
	}

	bounds_ = valui32;
	mark_transform_dirty();
}

size<uint32_t> Node::bounds() const {
	return bounds_;
}

uint32_t Node::width() const {
	return bounds_.width;
}

uint32_t Node::height() const {
	return bounds_.height;
}

void Node::set_position(vec2<float> val) {
	if (position_ == val) {
		return;
	}

	position_ = val;
	mark_transform_dirty();
}

void Node::set_position_x(float val) {
	if (position_.x == val) {
		return;
	}

	position_.x = val;
	mark_transform_dirty();
}

void Node::set_position_y(float val) {
	if (position_.y == val) {
		return;
	}

	position_.y = val;
	mark_transform_dirty();
}

vec2<float> Node::position() const {
	return position_;
}

void Node::set_anchor(vec2<float> val) {
	if (anchor_ == val) {
		return;
	}

	anchor_ = util::clamp(val, vec2<float>(0.f), vec2<float>(1.f));
	mark_transform_dirty();
}

vec2<float> Node::anchor() const {
	return anchor_;
}

void Node::set_scale(vec2<float> val) {
	if (scale_ == val) {
		return;
	}

	scale_ = val;
	mark_transform_dirty();
}

void Node::set_scale(float val) {
	if (scale_.x == val && scale_.y == val) {
		return;
	}

	scale_ = vec2<float>(val);
	mark_transform_dirty();
}

void Node::set_scale_x(float val) {
	if (scale_.x == val) {
		return;
	}

	scale_.x = val;
	mark_transform_dirty();
}

void Node::set_scale_y(float val) {
	if (scale_.y == val) {
		return;
	}

	scale_.y = val;
	mark_transform_dirty();
}

vec2<float> Node::scale() const {
	return scale_;
}

void Node::set_skew(vec2<float> val) {
	if (skew_ == val) {
		return;
	}

	skew_ = val;
	mark_transform_dirty();
}

vec2<float> Node::skew() const {
	return skew_;
}

void Node::set_rotation(float val) {
	if (rotation_ == val) {
		return;
	}

	rotation_ = val;
	mark_transform_dirty();
}

float Node::rotation() const {
	return rotation_;
}

void Node::set_color(rgba val) {
	if (color_ == val) {
		return;
	}

	color_ = val;
	mark_rgba_dirty();
}

rgba Node::color() const {
	return color_;
}

void Node::set_alpha(float val) {
	val                  = 255.f * std::clamp(val, 0.f, 1.f);
	uint8_t const valui8 = (uint8_t)std::round(val);

	if (color_.a == valui8) {
		return;
	}

	color_.a = valui8;
	mark_rgba_dirty();
}

float Node::alpha() const {
	return color_.a / 255.f;
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

void Node::toggle_flip(bool val) {
	if (is_flip_x_ == val && is_flip_y_ == val) {
		return;
	}

	is_flip_x_ = val;
	is_flip_y_ = val;
	mark_transform_dirty();
}

void Node::toggle_flip_x(bool val) {
	if (is_flip_x_ == val) {
		return;
	}

	is_flip_x_ = val;
	mark_transform_dirty();
}

bool Node::is_flip_x() const {
	return is_flip_x_;
}

void Node::toggle_flip_y(bool val) {
	if (is_flip_y_ == val) {
		return;
	}

	is_flip_y_ = val;
	mark_transform_dirty();
}

bool Node::is_flip_y() const {
	return is_flip_y_;
}

std::vector<std::shared_ptr<Node>> Node::children() const {
	return children_;
}

// protected
bool Node::init() {
	return true;
}

// protected
void Node::update(float dt) {}

// protected
void Node::draw(mat3 const& transform, rgba color) {}

// private
bool Node::init_node() {
	return init();
}

// private
void Node::update_all(float dt) {
	float const world_dt = dt * time_scale_;

	if (is_active_) {
		update(world_dt);
	}

	for (auto const& node : children_) {
		if (!node) {
			continue;
		}

		node->update_all(world_dt);
	}
}

// private
void Node::draw_all() {
	if (!is_visible_) {
		return;
	}

	if (is_rgba_dirty_) {
		combined_color_ = calculate_combined_rgba(parent_);
		is_rgba_dirty_  = false;
	}

	if (combined_color_.a == 0) {
		return;
	}

	if (is_transform_dirty_) {
		transform_          = calculate_transform(parent_);
		is_transform_dirty_ = false;
	}

	if (is_draw_scheduled_) {
		draw(transform_, combined_color_);
	}

	for (auto const& node : children_) {
		if (!node) {
			continue;
		}

		node->draw_all();
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
void Node::mark_rgba_dirty() {
	if (is_rgba_dirty_) {
		// already dirty
		return;
	}

	is_rgba_dirty_ = true;

	for (auto& child : children_) {
		if (child) {
			child->mark_rgba_dirty();
		}
	}
}

// private
mat3 Node::calculate_transform(std::weak_ptr<Node> parent) const {
	vec2<float> const anchor_position = {anchor_.x * bounds_.width, anchor_.y * bounds_.height};
	vec2<float> const skew_rad        = {util::degrees_to_radians(skew_.x), util::degrees_to_radians(skew_.y)};
	vec2<float> const scale_factor    = {is_flip_x_ ? -1.f : 1.f, is_flip_y_ ? -1.f : 1.f};
	mat3 const t                      = mat3::translation(position_);
	mat3 const r                      = mat3::rotation(util::degrees_to_radians(rotation_));
	mat3 const s                      = mat3::scale(scale_ * scale_factor);
	mat3 const k                      = mat3::skew(skew_rad);
	mat3 const a                      = mat3::translation(-anchor_position);
	mat3 const local                  = t * r * s * k * a;

	if (auto p = parent.lock()) {
		return p->transform_ * local;
	}

	return local;
}

// private
rgba Node::calculate_combined_rgba(std::weak_ptr<Node> parent) const {
	if (auto p = parent.lock()) {
		return p->color() * color_;
	}

	return color_;
}

} // namespace aether