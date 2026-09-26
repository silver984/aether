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
		unique_ref<T> out = ref::unique<T>(ctx, std::forward<Args>(args)...);
		if (!out->init_interface_()) {
			return nullptr;
		}
		return out;
	}

	bool add_child(strong_ref<node> n);

	[[nodiscard]] strong_ref<node> root_node() const { return root_node_; }

protected:
	virtual bool init_();
	virtual void update_(float dt) {}
	virtual void visit_() {}

	context const& ctx_;

private:
	inline bool init_interface_() { return init_(); }
	void update_all_(float dt);
	void draw_all_();

	// camera camera_; // todo

	strong_ref<node> root_node_;
};

} // namespace aether