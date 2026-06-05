#include <Context.hh>
#include <cassert>
#include <objects/Node.hh>
#include <objects/abstract/Scene.hh>
#include <services/core/Window.hh>

namespace aether {

Scene::Scene(Context const& ctx)
    : ctx_(ctx)
    , root_node_(Node::create<Node>(ctx))
    , is_active_(false)
    , is_visit_scheduled_(false) {
	assert(root_node_ != nullptr);
	size<uint32_t> const window_size = ctx.core_services.window.default_size();
	root_node_->set_bounds(static_cast<size<int>>(window_size));
	root_node_->set_position(window_size / 2.f);
}

Scene::~Scene() = default;

// protected
bool Scene::init() {
	return true;
}

// protected
void Scene::update(float dt) {}

// protected
void Scene::visit() {}

// protected
void Scene::activate() {
	is_active_ = true;
}

// protected
void Scene::deactivate() {
	is_active_ = false;
}

// protected
void Scene::schedule_visit() {
	is_visit_scheduled_ = true;
}

// protected
void Scene::unschedule_visit() {
	is_visit_scheduled_ = false;
}

// protected
void Scene::add(std::shared_ptr<Node> node) {
	root_node_->add_child(node);
}

// private
bool Scene::init_scene() {
	return init();
}

// private
void Scene::update_all(float dt) {
	root_node_->update_all(dt);

	if (is_active_) {
		update(dt);
	}
}

// private
void Scene::draw_all() {
	root_node_->draw_all();

	if (is_visit_scheduled_) {
		visit();
	}
}

} // namespace aether