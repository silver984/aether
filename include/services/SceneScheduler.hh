#pragma once
#include <memory>

namespace aether {

class Aether;
class Scene;

class SceneScheduler final {
	friend class Aether;

private:
	SceneScheduler();

public:
	~SceneScheduler();
	SceneScheduler(SceneScheduler const&)            = delete;
	SceneScheduler(SceneScheduler&&)                 = delete;
	SceneScheduler& operator=(SceneScheduler const&) = delete;
	SceneScheduler& operator=(SceneScheduler&&)      = delete;

	void replace_scene(std::shared_ptr<Scene>&& new_scene);

private:
	void cleanup();
	void update_scene(float dt);
	void draw_scene();
	[[nodiscard]] bool has_pending_scene() const;

	std::shared_ptr<Scene> current_scene_;
	std::shared_ptr<Scene> pending_scene_;
};

} // namespace aether