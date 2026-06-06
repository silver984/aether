#pragma once
#include <concepts>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

namespace aether {

class SceneScheduler;
class Context;
class Node;
class Sound;

class Scene {
	friend class SceneScheduler;

public:
	Scene(Context const& ctx);
	virtual ~Scene();

	template <typename Derived, typename... Args>
	    requires std::derived_from<Derived, Scene>
	[[nodiscard]] static std::unique_ptr<Derived> create(Context const& ctx, Args&&... args) {
		std::unique_ptr<Derived> ptr = std::make_unique<Derived>(ctx, std::forward<Args>(args)...);

		if (!ptr->init_scene()) {
			return nullptr;
		}

		return ptr;
	}

	void activate();
	void deactivate();
	void schedule_visit();
	void unschedule_visit();
	bool add(std::shared_ptr<Node> node);
	void add(std::shared_ptr<Sound> sound);
	[[nodiscard]] std::shared_ptr<Node> root_node() const;

protected:
	virtual bool init();
	virtual void update(float dt);
	virtual void visit();
	[[nodiscard]] Context const& ctx() const;

private:
	bool init_scene();
	void update_all(float dt);
	void draw_all();

	// TODO: music member
	// TODO: camera member
	// TODO: make nodes aware of the scene

	Context const& ctx_;
	std::shared_ptr<Node> root_node_;
	std::vector<std::shared_ptr<Sound>> sounds_;
	bool is_active_;
	bool is_visit_scheduled_;
};

} // namespace aether