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

	void switch_state(std::shared_ptr<Node>&& new_state);

private:
	void cleanup();
	void update_current_state(float dt);
	void draw_current_state();
	[[nodiscard]] bool has_pending_state() const;

	std::shared_ptr<Node> current_state_;
	std::shared_ptr<Node> pending_state_;
};

} // namespace aether