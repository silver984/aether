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
concept component_type_ =
        std::derived_from<T, node_component> && !std::same_as<T, node_component> && std::constructible_from<T, context const&, node*>;
} // namespace aether::_node_comp_impl

namespace aether {

class node_component {
	friend class node;

public:
	node_component(context const& ctx, aether::node* node)
	        : ctx_(ctx)
	        , node_(node) {}

	virtual ~node_component() = default;

	template <_node_comp_impl::component_type_ T>
	[[nodiscard]] static unique_ref<T> create(context const& ctx, aether::node* node) {
		unique_ref<T> out = ref::unique<T>(ctx, node);
		if (!out || !out->init_interface_()) {
			return nullptr;
		}
		return out;
	}

	[[nodiscard]] aether::node* node() const { return node_; }

protected:
	virtual void node_parented_() {}
	virtual void node_detached_() {}

	virtual bool init_() { return true; }
	virtual void update_(float dt) {}
	virtual void visit_() {}
	virtual void draw_() {}

	context const& ctx_;

private:
	bool init_interface_() { return init_(); }

	aether::node* node_;
};

} // namespace aether