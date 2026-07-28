#include <algorithm>
#include <cmath>
#include <debug/log.hh>
#include <nodes/node.hh>
#include <scene.hh>
#include <util/math.hh>

#include <cassert>

namespace aether {

node::node(context const& ctx_) noexcept
        : m_ctx_(ctx_)
        , scene_(nullptr)
        , parent_(nullptr)
        , color_(255)
        , combined_color_(color_)
        , transform_(mat3::identity())
        , anchor_(0.5f)
        , scale_(1.f)
        , scroll_factor_(1.f)
        , rotation_(0.f)
        , time_scale_(1.f)
        , is_flip_x_(false)
        , is_flip_y_(false)
        , is_transform_dirty_(false)
        , is_rgba_dirty_(false)
        , is_active_(false)
        , is_draw_scheduled_(false)
        , is_visible_(true) {
	AETHER_TRACELOG("creating {}", fmt::ptr(this));
}
node::~node() noexcept {
	AETHER_TRACELOG("destroying {}", fmt::ptr(this));
}

bool node::add_child(strong_ref<node> child) {
	if (!child) {
		return false;
	}

	auto self          = strong_this_();
	bool const is_self = child == self;

	if (is_self) {
		return false;
	}

	bool const has_ancestor = child->has_ancestor_(self);

	if (has_ancestor) {
		return false;
	}

	bool const is_duplicate = std::find(children_.begin(), children_.end(), child) != children_.end();

	if (is_duplicate) {
		return false;
	}

	if (auto old_parent = child->parent_.construct()) {
		old_parent->remove_child(child);
	}

	auto placed_child     = children_.emplace_back(child);
	placed_child->parent_ = this;
	placed_child->mark_transform_dirty_();
	placed_child->mark_rgba_dirty_();

	return true;
}

bool node::remove_child(strong_ref<node> child) {
	if (!child) {
		return false;
	}

	auto it = std::find(children_.begin(), children_.end(), child);

	if (it == children_.end()) {
		return false;
	}

	(*it)->parent_.detach();
	children_.erase(it);

	return true;
}

void node::destroy_all() {
	detach_from_parent();
	// recursive destroy
	while (!children_.empty()) {
		auto child = children_.back();
		children_.pop_back();
		child->parent_ = nullptr;
		child->destroy_all();
	}
}

bool node::detach_from_parent() {
	if (auto p = parent_.construct()) {
		return p->remove_child(strong_this_());
	}
	return false;
}

void node::activate() {
	AETHER_TRACELOG("activated from {}", fmt::ptr(this));
	is_active_ = true;
}

void node::deactivate() {
	is_active_ = false;
}

void node::schedule_draw() {
	is_draw_scheduled_ = true;
}

void node::unschedule_draw() {
	is_draw_scheduled_ = false;
}

size_t node::child_count() const {
	return children_.size();
}

size_t node::recursed_child_count() const {
	size_t c = children_.size();
	for (auto const& child : children_) {
		if (!child) {
			continue;
		}
		c += child->recursed_child_count();
	}
	return c;
}

weak_ref<node> node::parent() const {
	return parent_;
}

void node::set_name(std::string_view name) {
	if (name_ == name) {
		return;
	}
	name_ = std::string(name);
}

std::string_view node::name() const {
	return name_;
}

void node::set_bounds(size<int> val) {
	val = util::max(size<int>(0), val);
	if (bounds_ == val) {
		return;
	}
	bounds_ = val;
	mark_transform_dirty_();
}

size<int> node::bounds() const {
	return bounds_;
}

int node::width() const {
	return bounds_.width;
}

int node::height() const {
	return bounds_.height;
}

void node::set_position(vec2<float> val) {
	if (position_ == val) {
		return;
	}
	position_ = val;
	mark_transform_dirty_();
}

void node::set_position_x(float val) {
	if (position_.x == val) {
		return;
	}
	position_.x = val;
	mark_transform_dirty_();
}

void node::set_position_y(float val) {
	if (position_.y == val) {
		return;
	}
	position_.y = val;
	mark_transform_dirty_();
}

vec2<float> node::position() const {
	return position_;
}

void node::set_anchor(vec2<float> val) {
	if (anchor_ == val) {
		return;
	}
	anchor_ = util::clamp(val, vec2<float>(0.f), vec2<float>(1.f));
	mark_transform_dirty_();
}

vec2<float> node::anchor() const {
	return anchor_;
}

void node::set_scale(vec2<float> val) {
	if (scale_ == val) {
		return;
	}
	scale_ = val;
	mark_transform_dirty_();
}

void node::set_scale(float val) {
	if (scale_.x == val && scale_.y == val) {
		return;
	}
	scale_ = vec2<float>(val);
	mark_transform_dirty_();
}

void node::set_scale_x(float val) {
	if (scale_.x == val) {
		return;
	}
	scale_.x = val;
	mark_transform_dirty_();
}

void node::set_scale_y(float val) {
	if (scale_.y == val) {
		return;
	}
	scale_.y = val;
	mark_transform_dirty_();
}

vec2<float> node::scale() const {
	return scale_;
}

void node::set_skew(vec2<float> val) {
	if (skew_ == val) {
		return;
	}
	skew_ = val;
	mark_transform_dirty_();
}

vec2<float> node::skew() const {
	return skew_;
}

void node::set_scroll_factor(vec2<float> val) {
	if (scroll_factor_ == val) {
		return;
	}
	scroll_factor_ = val;
	mark_transform_dirty_();
}

vec2<float> node::scroll_factor() const {
	return scroll_factor_;
}

void node::set_rotation(float val) {
	if (rotation_ == val) {
		return;
	}
	rotation_ = val;
	mark_transform_dirty_();
}

float node::rotation() const {
	return rotation_;
}

void node::set_color(rgba val) {
	if (color_ == val) {
		return;
	}
	color_ = val;
	mark_rgba_dirty_();
}

rgba node::color() const {
	return color_;
}

void node::set_alpha(float val) {
	val                  = 255.f * std::clamp(val, 0.f, 1.f);
	uint8_t const valui8 = (uint8_t)std::round(val);
	if (color_.a == valui8) {
		return;
	}
	color_.a = valui8;
	mark_rgba_dirty_();
}

float node::alpha() const {
	return color_.a / 255.f;
}

void node::toggle_visibility(bool val) {
	is_visible_ = val;
}

bool node::is_visible() const {
	return is_visible_;
}

void node::set_time_scale(float val) {
	time_scale_ = std::max(0.f, val);
}

float node::time_scale() const {
	return time_scale_;
}

void node::toggle_flip(bool val) {
	if (is_flip_x_ == val && is_flip_y_ == val) {
		return;
	}
	is_flip_x_ = val;
	is_flip_y_ = val;
	mark_transform_dirty_();
}

void node::toggle_flip_x(bool val) {
	if (is_flip_x_ == val) {
		return;
	}
	is_flip_x_ = val;
	mark_transform_dirty_();
}

bool node::is_flip_x() const {
	return is_flip_x_;
}

void node::toggle_flip_y(bool val) {
	if (is_flip_y_ == val) {
		return;
	}
	is_flip_y_ = val;
	mark_transform_dirty_();
}

bool node::is_flip_y() const {
	return is_flip_y_;
}

std::vector<strong_ref<node>> node::children() const {
	return children_;
}

bool node::init_() {
	return true;
}

void node::update_(float dt) {
}

void node::draw_(mat3 const& transform, rgba color) {
}

scene* node::get_scene() const {
	if (scene_) {
		return scene_;
	}
	auto p = parent_.construct();
	return p ? p->get_scene() : nullptr;
}

context const& node::ctx_() const {
	return m_ctx_;
}

bool node::init_interface_() {
	return init_();
}

void node::update_all_(float dt) {
	AETHER_TRACELOG("this={} active={}", fmt::ptr(this), is_active_);
	float const world_dt = dt * time_scale_;
	if (is_active_) {
		update_(world_dt);
	}
	for (auto const& node : children_) {
		if (!node) {
			continue;
		}
		node->update_all_(world_dt);
	}
}

void node::draw_all_() {
	if (!is_visible_) {
		return;
	}
	if (is_rgba_dirty_) {
		combined_color_ = calculate_combined_rgba_();
		is_rgba_dirty_  = false;
	}
	if (combined_color_.a == 0) {
		return;
	}
	if (is_transform_dirty_) {
		transform_          = calculate_transform_();
		is_transform_dirty_ = false;
	}
	if (is_draw_scheduled_) {
		draw_(transform_, combined_color_);
	}
	for (auto const& node : children_) {
		if (!node) {
			continue;
		}
		node->draw_all_();
	}
}

bool node::has_ancestor_(strong_ref<node> child) const {
	auto p = parent_.construct();
	while (p) {
		if (p == child) {
			return true;
		}
		p = p->parent_.construct();
	}
	return false;
}

void node::mark_transform_dirty_() {
	if (is_transform_dirty_) {
		// already dirty
		return;
	}
	is_transform_dirty_ = true;
	for (auto& child : children_) {
		child->mark_transform_dirty_();
	}
}

void node::mark_rgba_dirty_() {
	if (is_rgba_dirty_) {
		// already dirty
		return;
	}
	is_rgba_dirty_ = true;
	for (auto& child : children_) {
		child->mark_rgba_dirty_();
	}
}

mat3 node::calculate_transform_() const {
	// todo: use scene camera
	vec2<float> const anchor_position = vec2<float>(anchor_.x * bounds_.width, anchor_.y * bounds_.height);
	vec2<float> const skew_rad        = vec2<float>(util::degrees_to_radians(skew_.x), util::degrees_to_radians(skew_.y));
	vec2<float> const scale_factor    = vec2<float>(is_flip_x_ ? -1.f : 1.f, is_flip_y_ ? -1.f : 1.f);

	mat3 const t     = mat3::translation(position_ * scroll_factor_);
	mat3 const r     = mat3::rotation(util::degrees_to_radians(rotation_));
	mat3 const s     = mat3::scale(scale_ * scale_factor);
	mat3 const k     = mat3::skew(skew_rad);
	mat3 const a     = mat3::translation(-anchor_position);
	mat3 const local = t * r * s * k * a;

	if (auto p = parent_.construct()) {
		return p->transform_ * local;
	}

	return local;
}

rgba node::calculate_combined_rgba_() const {
	if (auto p = parent_.construct()) {
		return p->color_ * color_;
	}
	return color_;
}

} // namespace aether