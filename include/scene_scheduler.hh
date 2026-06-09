#pragma once
#include <memory>

namespace aether {

class game;
class scene;

class scene_scheduler final {
	friend class game;

public:
	~scene_scheduler();
	scene_scheduler(scene_scheduler const&)            = delete;
	scene_scheduler(scene_scheduler&&)                 = delete;
	scene_scheduler& operator=(scene_scheduler const&) = delete;
	scene_scheduler& operator=(scene_scheduler&&)      = delete;
	void replace_scene(std::unique_ptr<scene> new_scene);

private:
	scene_scheduler();
	void cleanup_();
	void update_scene_(float dt);
	void draw_scene_();
	[[nodiscard]] bool has_pending_scene_() const;

	std::unique_ptr<scene> current_scene_;
	std::unique_ptr<scene> pending_scene_;
};

} // namespace aether