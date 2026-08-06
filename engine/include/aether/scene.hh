#pragma once
// #include <camera.hh>
#include <aether/ref.hh>
#include <concepts>
#include <memory>
#include <utility>

namespace aether {

class scene_scheduler;
class context;
class node;

class scene {
	friend class scene_scheduler;

public:
	scene(context const& ctx) noexcept;
	virtual ~scene() noexcept;

	template <typename T, typename... va>
	        requires std::derived_from<T, scene>
	[[nodiscard]] static std::unique_ptr<T> create(context const& ctx, va&&... args) {
		std::unique_ptr<T> ptr = std::make_unique<T>(ctx, std::forward<va>(args)...);
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

	// [[nodiscard]] camera& get_camera();

protected:
	virtual bool init_();
	virtual void update_(float dt);
	virtual void visit_();

	[[nodiscard]] context const& ctx_() const;

private:
	bool init_interface_();

	void update_all_(float dt);
	void draw_all_();

	// todo: music member

	context const& m_ctx_;

	// camera camera_; // todo

	strong_ref<node> root_node_;

	bool is_active_;
	bool is_visit_scheduled_;
};

} // namespace aether