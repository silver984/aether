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
	scene(context const& ctx) noexcept;
	virtual ~scene() noexcept;

	template <std::derived_from<scene> T, typename... Args>
	[[nodiscard]] static unique_ref<T> create(context const& ctx, Args&&... args) {
		auto ptr = unique_ref<T>::create(ctx, std::forward<Args>(args)...);
		if (!ptr->init_interface_()) {
			return nullptr;
		}
		return ptr;
	}

	void activate();
	void deactivate();

	void schedule_visit();
	void unschedule_visit();

	bool add(strong_ref<node> n);

	[[nodiscard]] weak_ref<node> root_node() const;

protected:
	virtual bool init_();
	virtual void update_(float dt);
	virtual void visit_();

	context const& ctx_;

private:
	bool init_interface_();

	void update_all_(float dt);
	void draw_all_();

	// camera camera_; // todo

	strong_ref<node> root_node_;

	bool is_active_;
	bool is_visit_scheduled_;
};

} // namespace aether