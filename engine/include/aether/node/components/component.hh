#pragma once
#include <aether/ref.hh>
#include <concepts>

namespace aether {

class node;
class context;
class node_component {
	friend class node;

public:
	node_component(context const& ctx, strong_ref<node> n)
	        : ctx_(ctx)
	        , weak_node_(n) {}
	virtual ~node_component() = default;

	template <typename T, typename... Args>
	        requires std::derived_from<T, node_component>
	[[nodiscard]] static unique_ref<T> create(context const& ctx, strong_ref<node> n, Args&&... args) {
		auto out = unique_ref<T>::create(ctx, n, std::forward<Args>(args)...);
		if (!out->init_interface_()) {
			return nullptr;
		}
		return out;
	}

protected:
	virtual void node_pushed_() {}
	virtual bool init_() { return true; }
	virtual void update_(float dt) {}
	virtual void visit_() {}
	virtual void draw_() {}

	[[nodiscard]] strong_ref<node> strong_node_() const { return weak_node_.construct(); }

	context const& ctx_;
	weak_ref<node> weak_node_;

private:
	bool init_interface_() { return init_(); }
};

template <typename T>
concept node_component_type = std::derived_from<T, node_component> && !std::same_as<T, node_component>;

} // namespace aether