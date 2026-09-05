#pragma once
#include <aether/ref.hh>
#include <concepts>

namespace aether {

class node_component;
class node;
class context;

} // namespace aether

namespace aether::_node_comp_impl {

template <typename T>
concept comp_ = std::derived_from<T, node_component> && !std::same_as<T, node_component> &&
                std::constructible_from<T, context const&, strong_ref<node>>;

template <_node_comp_impl::comp_... T>
struct dep_comps_ final {
	using dep_comps_ide_ = void;
};

template <typename T>
concept has_deps_ = comp_<T> && requires {
	typename T::dependencies;
	typename T::dependencies::dep_comps_ide_;
};

using type_id_ = void const*;

template <typename T>
constexpr char id_{};

template <typename T>
constexpr type_id_ type_id_v_ = &id_<T>;

} // namespace aether::_node_comp_impl

namespace aether {

template <_node_comp_impl::comp_... T>
using node_component_list = _node_comp_impl::dep_comps_<T...>;

class node_component {
	friend class node;

public:
	node_component(context const& ctx, strong_ref<node> n)
	        : ctx_(ctx)
	        , weak_node_(n) {}
	virtual ~node_component() = default;

	template <_node_comp_impl::comp_ T>
	[[nodiscard]] static unique_ref<T> create(context const& ctx, strong_ref<node> n) {
		auto out = unique_ref<T>::create(ctx, n);
		out->init_interface_();
		return out;
	}

protected:
	virtual void node_parented_() {}
	virtual void node_detached_() {}
	virtual void init_() {}
	virtual void update_(float dt) {}
	virtual void visit_() {}
	virtual void draw_() {}

	[[nodiscard]] strong_ref<node> strong_node_() const { return weak_node_.construct(); }

	context const& ctx_;
	weak_ref<node> weak_node_;

private:
	void init_interface_() { init_(); }
};

} // namespace aether