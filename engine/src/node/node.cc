#include <aether/math.hh>
#include <aether/node/node.hh>
#include <aether/scene.hh>
#include <algorithm>

namespace aether {

bool node::add_child(strong_ref<node> child) {
	if (!child) {
		return false;
	}

	strong_ref<node> self = this->strong_self_();
	if (child == self || child->has_ancestor_(self)) {
		return false;
	}

	bool const is_duplicate = std::find(children_.begin(), children_.end(), child) != children_.end();
	if (is_duplicate) {
		return false;
	}

	child->detach_from_parent();
	children_.emplace_back(child);
	child->parent_ = self;
	for (auto& component : child->components_) {
		component->node_parented_();
	}

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
	for (auto& component : child->components_) {
		component->node_detached_();
	}

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
	if (strong_ref<node> p = parent_.construct()) {
		return p->remove_child(this->strong_self_());
	}
	return false;
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

void node::set_name(std::string_view name) {
	if (name_ == name) {
		return;
	}
	name_ = std::string(name);
}

// void node::set_color(rgba val) {
// 	if (color_ == val) {
// 		return;
// 	}
// 	color_ = val;
// 	mark_rgba_dirty_();
// }

// rgba node::color() const { return color_; }

// void node::set_alpha(float val) {
// 	val                  = 255.f * std::clamp(val, 0.f, 1.f);
// 	uint8_t const valui8 = (uint8_t)std::round(val);
// 	if (color_.a == valui8) {
// 		return;
// 	}
// 	color_.a = valui8;
// 	mark_rgba_dirty_();
// }

// float node::alpha() const { return color_.a / 255.f; }

aether::scene* node::scene() const {
	if (scene_) {
		return scene_;
	}
	auto p = parent_.construct();
	return p ? p->scene() : nullptr;
}

void node::update_(float dt) {
	for (auto& component : components_) {
		component->update_(dt);
	}
	for (auto& child : children_) {
		child->update_(dt);
	}
}

void node::draw_() {
	// if (is_rgba_dirty_) {
	// 	combined_color_ = calculate_combined_rgba_();
	// 	is_rgba_dirty_  = false;
	// }
	// if (combined_color_.a == 0) {
	// 	return;
	// }

	for (auto& component : components_) {
		component->visit_();
	}
	for (auto& component : components_) {
		component->draw_();
	}
	for (auto& child : children_) {
		child->draw_();
	}
}

bool node::has_ancestor_(strong_ref<node> child) const {
	strong_ref<node> p = parent_.construct();
	while (p) {
		if (p == child) {
			return true;
		}
		p = p->parent_.construct();
	}
	return false;
}

} // namespace aether