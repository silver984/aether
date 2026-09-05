#pragma once
#include <aether/context.hh>
#include <aether/log.hh>
#include <aether/mat3.hh>
#include <aether/node/components/component.hh>
#include <aether/ref.hh>
#include <aether/rgba.hh>
#include <aether/size.hh>
#include <aether/vec2.hh>

#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>
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

	[[nodiscard]] static strong_ref<node> create(context const& ctx);

	// todo: fetch child
	bool add_child(strong_ref<node> child);
	bool remove_child(strong_ref<node> child);

	void destroy_all();

	bool detach_from_parent();

	template <_node_comp_impl::comp_ T>
	T* add_component() {
		if (T* existing = component<T>()) {
			return existing;
		}
		if constexpr (_node_comp_impl::has_deps_<T>) {
			ensure_dep_comps_(typename T::dependencies{});
		}
		unique_ref<T> c = node_component::create<T>(ctx_, this->strong_self_());
		auto [it, _]    = components_.emplace(_node_comp_impl::type_id_v_<T>, std::move(c));
		return static_cast<T*>(it->second.get());
	}

	template <_node_comp_impl::comp_ T>
	bool remove_component() {
		auto it = components_.find(_node_comp_impl::type_id_v_<T>);
		if (it == components_.end()) {
			return false;
		}
		components_.erase(it);
		return false;
	}

	template <_node_comp_impl::comp_ T>
	[[nodiscard]] T* component() const {
		auto it = components_.find(_node_comp_impl::type_id_v_<T>);
		if (it == components_.end()) {
			return nullptr;
		}
		return static_cast<T*>(it->second.get());
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

	template <_node_comp_impl::comp_... T>
	void ensure_dep_comps_(node_component_list<T...>) {
		(add_component<T>(), ...);
	}

	// void mark_rgba_dirty_();
	// [[nodiscard]] rgba calculate_combined_rgba_() const;

	context const& ctx_;

	aether::scene* scene_;
	weak_ref<node> parent_;
	std::vector<strong_ref<node>> children_;
	std::unordered_map<_node_comp_impl::type_id_, unique_ref<node_component>> components_;

	std::string name_;

	// rgba color_;
	// rgba combined_color_;
	// bool is_rgba_dirty_;
};

} // namespace aether