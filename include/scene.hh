#pragma once
// #include <camera.hh>
#include <concepts>
#include <memory>
#include <util/ref.hh>
#include <utility>
#include <vector>

namespace aether {

class scene_scheduler;
class context;
class node;
class sound;

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

	bool add(ref<node> n);
	bool add(ref<sound> s);

	[[nodiscard]] ref<node> root() const;

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

	ref<node> root_;

	std::vector<ref<sound>> sounds_;

	bool is_active_;
	bool is_visit_scheduled_;
};

} // namespace aether