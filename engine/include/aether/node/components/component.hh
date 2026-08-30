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
concept component = std::derived_from<T, node_component> && !std::same_as<T, node_component> &&
                    std::constructible_from<T, context const&, strong_ref<node>>;

template <_node_comp_impl::component... T>
struct required_components {
	using required_components_identifier = void;
};

template <typename T>
concept has_requirements = component<T> && requires {
	typename T::requirements;
	typename T::requirements::required_components_identifier;
};

} // namespace aether::_node_comp_impl

namespace aether {

template <_node_comp_impl::component... T>
using node_component_list = _node_comp_impl::required_components<T...>;

class node_component {
	friend class node;

public:
	node_component(context const& ctx, strong_ref<node> n)
	        : ctx_(ctx)
	        , weak_node_(n) {}
	virtual ~node_component() = default;

	template <_node_comp_impl::component T>
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
	void init_interface_() { return init_(); }
};

} // namespace aether