#pragma once
#include <aether/general.h>
#include <memory>

namespace aether {

class game;
class scene;

} // namespace aether

namespace aether::core {

class scene_scheduler final {
	friend class aether::game;

public:
	~scene_scheduler();
	DELETE_COPY_AND_MOVE(scene_scheduler);
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

} // namespace aether::core