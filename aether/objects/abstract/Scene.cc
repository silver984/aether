#include <Context.hh>
#include <log.hh>
#include <objects/abstract/Scene.hh>
#include <services/core/Window.hh>

namespace aether {

Scene::Scene(Context const& ctx)
    : Node(ctx)
    , work_step_(0)
    , is_ready_(false)
    , entered_(false) {
	size<std::uint32_t> const window_size = ctx.core_services.window.default_size();
	set_bounds(static_cast<size<int>>(window_size));
	set_position(window_size / 2.f);
}

Scene::~Scene() = default;

std::string_view Scene::type() const {
	return "Scene";
}

// protected
void Scene::enter() {}

// protected
void Scene::queue_work(std::function<void()>&& workload) {
	if (is_ready_) {
		return;
	}

	work_queue_.emplace_back(std::move(workload));
}

// private
void Scene::base_enter() {
	enter();
	entered_ = true;
}

// private
void Scene::step_work_queue() {
	if (is_ready_) {
		return;
	}

	if (work_step_ < work_queue_.size()) {
		work_queue_[work_step_++](); // run workload
		return;
	}

	work_queue_.clear();
	is_ready_ = true;
}

bool Scene::is_ready() const {
	return is_ready_;
}

bool Scene::entered() const {
	return entered_;
}

} // namespace aether