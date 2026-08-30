#pragma once
#include <aether/ref.hh>
#include <concepts>
#include <utility>

namespace aether {

class scene_scheduler;
class node;
class context;

class scene {
	friend class scene_scheduler;

public:
	scene(context const& ctx)
	        : ctx_(ctx)
	        , root_node_(nullptr) {}
	virtual ~scene() = default;

	template <std::derived_from<scene> T, typename... Args>
	[[nodiscard]] static unique_ref<T> create(context const& ctx, Args&&... args) {
		auto ptr = unique_ref<T>::create(ctx, std::forward<Args>(args)...);
		if (!ptr->init_interface_()) {
			return nullptr;
		}
		return ptr;
	}

	bool add_child(strong_ref<node> n);

	[[nodiscard]] weak_ref<node> root_node() const { return root_node_; }

protected:
	virtual bool init_();
	virtual void update_(float dt) {}
	virtual void visit_() {}

	context const& ctx_;

private:
	bool init_interface_() { return init_(); }
	void update_all_(float dt);
	void draw_all_();

	// camera camera_; // todo

	strong_ref<node> root_node_;
};

} // namespace aether