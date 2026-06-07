#pragma once
#include <abstract/camera.hh>
#include <concepts>
#include <memory>
#include <string_view>
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
	scene(context const& ctx);
	virtual ~scene();

	template <typename derived, typename... va>
	    requires std::derived_from<derived, scene>
	[[nodiscard]] static std::unique_ptr<derived> create(context const& ctx, va&&... args) {
		std::unique_ptr<derived> ptr = std::make_unique<derived>(ctx, std::forward<va>(args)...);

		if (!ptr->init_scene()) {
			return nullptr;
		}

		return ptr;
	}

	void activate();
	void deactivate();
	void schedule_visit();
	void unschedule_visit();
	bool add(std::shared_ptr<node> _node);
	bool add(std::shared_ptr<sound> _sound);
	[[nodiscard]] std::shared_ptr<node> root_node() const;
	[[nodiscard]] camera& fetch_camera();

protected:
	virtual bool init();
	virtual void update(float dt);
	virtual void visit();
	[[nodiscard]] context const& ctx() const;

private:
	bool init_scene();
	void update_all(float dt);
	void draw_all();

	// TODO: music member
	// TODO: camera member

	context const& ctx_;
	camera camera_;
	std::shared_ptr<node> root_node_;
	std::vector<std::shared_ptr<sound>> sounds_;
	bool is_active_;
	bool is_visit_scheduled_;
};

} // namespace aether