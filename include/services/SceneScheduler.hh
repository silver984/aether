#pragma once
#include <memory>
#include <objects/Node.hh>

namespace aether {

class Aether;

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

	void replace_scene(std::shared_ptr<Node>&& new_scene);

private:
	void cleanup();
	void update_scene(float dt);
	void draw_scene();
	[[nodiscard]] bool has_pending_scene() const;

	std::shared_ptr<Node> current_scene_;
	std::shared_ptr<Node> pending_scene_;
};

} // namespace aether