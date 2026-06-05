#pragma once
#include <functional>
#include <objects/Node.hh>

namespace aether {

class SceneScheduler;

class Scene : public Node {
	friend class SceneScheduler;

public:
	Scene(Context const& ctx);
	~Scene() override;

	void destroy()                  = delete;
	void set_name(std::string_view) = delete;
	std::string_view name() const   = delete;
	[[nodiscard]] std::string_view type() const override;

protected:
	virtual void enter();
	void queue_work(std::function<void()>&& workload);

private:
	using Node::set_bounds;
	void base_enter();
	void step_work_queue();
	[[nodiscard]] bool is_ready() const;
	[[nodiscard]] bool entered() const;

	std::vector<std::function<void()>> work_queue_;
	size_t work_step_;
	bool is_ready_;
	bool entered_;
};

} // namespace aether