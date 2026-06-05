#pragma once
#include <concepts>
#include <memory>
#include <utility>

namespace aether {

class SceneScheduler;
class Context;
class Node;

class Scene {
	friend class SceneScheduler;

public:
	Scene(Context const& ctx);
	virtual ~Scene();

	template <typename T, typename... va>
	    requires std::derived_from<T, Scene>
	[[nodiscard]] static std::unique_ptr<T> create(Context const& ctx, va&&... args) {
		std::unique_ptr<T> ptr = std::make_unique<T>(ctx, std::forward<va>(args)...);

		if (!ptr->init_scene()) {
			return nullptr;
		}

		return ptr;
	}

protected:
	virtual bool init();
	virtual void update(float dt);
	virtual void visit();
	void activate();
	void deactivate();
	void schedule_visit();
	void unschedule_visit();
	void add(std::shared_ptr<Node> node);

	Context const& ctx_;

private:
	bool init_scene();
	void update_all(float dt);
	void draw_all();

	std::shared_ptr<Node> root_node_;
	bool is_active_;
	bool is_visit_scheduled_;
};

} // namespace aether