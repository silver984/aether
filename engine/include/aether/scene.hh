#pragma once
// #include <camera.hh>
#include <aether/ref.hh>
#include <concepts>
#include <memory>
#include <utility>

namespace aether::core {

class scene_scheduler;

}

namespace aether {

class context;
class node;

class scene {
	friend class core::scene_scheduler;

public:
	scene(context const& ctx) noexcept;
	virtual ~scene() noexcept;

	template <std::derived_from<scene> Type, typename... Args>
	[[nodiscard]] static std::unique_ptr<Type> create(context const& ctx, Args&&... args) {
		auto ptr = std::make_unique<Type>(ctx, std::forward<Args>(args)...);
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

	context const& ctx;

protected:
	virtual bool init_();
	virtual void update_(float dt);
	virtual void visit_();

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