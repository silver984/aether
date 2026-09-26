#pragma once

#include "components/component.hh"

#include <aether/context.hh>
#include <aether/mat3.hh>
#include <aether/ref.hh>
#include <aether/rgba.hh>
#include <aether/size.hh>
#include <aether/vec2.hh>

#include <cstddef>
#include <utility>
#include <vector>

namespace aether {

class scene;
class context;

class node final : public self_referenceable<node> {
	friend class scene;

public:
	node(context const& ctx)
	        : ctx_(ctx) {}

	[[nodiscard]] static strong_ref<node> create(context const& ctx) { return ref::strong<node>(ctx); }

	// todo: fetch child
	bool add_child(strong_ref<node> child);
	bool remove_child(strong_ref<node> const& child);

	void destroy_all();

	bool detach_from_parent();

	template <_node_comp_impl::component_type_ T>
	T* add_component() {
		if (T* existing = component<T>()) {
			return existing;
		}
		unique_ref<T> c = node_component::create<T>(ctx_, this);
		if (!c) {
			return nullptr;
		}
		auto& out = components_.emplace_back(std::move(c));
		return static_cast<T*>(out.get());
	}

	template <_node_comp_impl::component_type_ T>
	void remove_component() {
		std::erase_if(components_, [](unique_ref<node_component> const& component) {
			return dynamic_cast<T*>(component.get()) != nullptr;
		});
	}

	template <_node_comp_impl::component_type_ T>
	[[nodiscard]] T* component() const {
		for (auto& component : components_) {
			if (T* ptr = dynamic_cast<T*>(component.get())) {
				return ptr;
			}
		}
		return nullptr;
	}

	[[nodiscard]] size_t child_count() const { return children_.size(); }
	[[nodiscard]] size_t recursed_child_count() const;

	[[nodiscard]] strong_ref<node> parent() const { return parent_.construct(); }

	// todo: naming system

	// void set_color(rgba val);
	// [[nodiscard]] rgba color() const;

	// void set_alpha(float val);
	// [[nodiscard]] float alpha() const;

	[[nodiscard]] std::vector<strong_ref<node>> children() const { return children_; }
	[[nodiscard]] aether::scene* scene() const;

private:
	void update_(float dt);
	void draw_();

	[[nodiscard]] bool has_ancestor_(strong_ref<node> const& n) const;

	// void mark_rgba_dirty_();
	// [[nodiscard]] rgba calculate_combined_rgba_() const;

	context const& ctx_;

	aether::scene* scene_;
	weak_ref<node> parent_;
	std::vector<strong_ref<node>> children_;
	std::vector<unique_ref<node_component>> components_;

	// rgba color_;
	// rgba combined_color_;
	// bool is_rgba_dirty_;
};

} // namespace aether