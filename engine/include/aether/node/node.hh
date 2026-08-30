#pragma once
#include <aether/context.hh>
#include <aether/mat3.hh>
#include <aether/node/components/component.hh>
#include <aether/ref.hh>
#include <aether/rgba.hh>
#include <aether/size.hh>
#include <aether/vec2.hh>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace aether {

class scene;
class context;

class node final : public self_ref<node> {
	friend class scene;

public:
	node(context const& ctx)
	        : ctx_(ctx)
	        , scene_(nullptr)
	        , parent_(nullptr) {}
	~node() override = default;

	template <typename... Args>
	[[nodiscard]] static strong_ref<node> create(context const& ctx) {
		auto ptr = strong_ref<node>::create(ctx);
		return ptr;
	}

	// todo: fetch child

	bool add_child(strong_ref<node> child);
	bool remove_child(strong_ref<node> child);

	void destroy_all();

	bool detach_from_parent();

	template <node_component_type T, typename... Args>
	bool add_component(Args&&... args) { // todo: remove_component<T>
		if (component<T>()) {
			return true;
		}
		unique_ref<T> component = node_component::create<T>(ctx_, this->strong_self_(), std::forward<Args>(args)...);
		if (!component) {
			return false;
		}
		components_.emplace_back(std::move(component));
		return true;
	}

	template <node_component_type T>
	[[nodiscard]] T* component() const {
		for (auto& comp : components_) {
			if (T* ptr = dynamic_cast<T*>(comp.get())) {
				return ptr;
			}
		}
		return nullptr;
	}

	[[nodiscard]] size_t child_count() const { return children_.size(); }
	[[nodiscard]] size_t recursed_child_count() const;

	[[nodiscard]] weak_ref<node> parent() const { return parent_; }

	void set_name(std::string_view name); // todo: better naming system
	[[nodiscard]] std::string_view name() const { return name_; }

	// void set_color(rgba val);
	// [[nodiscard]] rgba color() const;

	// void set_alpha(float val);
	// [[nodiscard]] float alpha() const;

	[[nodiscard]] std::vector<strong_ref<node>> children() const { return children_; }
	[[nodiscard]] aether::scene* scene() const;

private:
	void update_(float dt);
	void draw_();

	[[nodiscard]] bool has_ancestor_(strong_ref<node> n) const;

	// void mark_rgba_dirty_();
	// [[nodiscard]] rgba calculate_combined_rgba_() const;

	context const& ctx_;

	aether::scene* scene_;
	weak_ref<node> parent_;
	std::vector<strong_ref<node>> children_;
	std::vector<unique_ref<node_component>> components_;

	std::string name_;

	// rgba color_;
	// rgba combined_color_;
	// bool is_rgba_dirty_;
};

} // namespace aether