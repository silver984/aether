#pragma once
#include <camera.hh>
#include <concepts>
#include <memory>
#include <ref.hh>
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

		if (!ptr->init_scene_()) {
			return nullptr;
		}

		return ptr;
	}

	void activate();
	void deactivate();
	void schedule_visit();
	void unschedule_visit();
	bool add(std::shared_ptr<node> n);
	bool add(ref<sound> s);
	[[nodiscard]] std::shared_ptr<node> root_node() const;
	[[nodiscard]] camera& get_camera();

protected:
	virtual bool init_();
	virtual void update_(float dt);
	virtual void visit_();
	[[nodiscard]] context const& ctx_() const;

private:
	bool init_scene_();
	void update_all_(float dt);
	void draw_all_();

	// todo: music member

	context const& mctx_;
	camera camera_; // todo
	std::shared_ptr<node> root_node_;
	std::vector<ref<sound>> sounds_;
	bool is_active_;
	bool is_visit_scheduled_;
};

} // namespace aether