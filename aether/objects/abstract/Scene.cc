#include <Context.hh>
#include <abstract/Scene.hh>
#include <abstract/Sound.hh>
#include <cassert>
#include <log.hh>
#include <objects/Node.hh>
#include <services/core/Window.hh>

namespace aether {

Scene::Scene(Context const& ctx)
    : ctx_(ctx)
    , camera_(root_node_)
    , root_node_(Node::create<Node>(ctx))
    , is_active_(false)
    , is_visit_scheduled_(false) {
	assert(root_node_ != nullptr);
	root_node_->scene_               = this;
	size<uint32_t> const window_size = ctx.core().window().default_size();
	root_node_->set_bounds(static_cast<size<int>>(window_size));
	root_node_->set_position(window_size / 2.f);
}

Scene::~Scene() = default;

void Scene::activate() {
	is_active_ = true;
}

void Scene::deactivate() {
	is_active_ = false;
}

void Scene::schedule_visit() {
	is_visit_scheduled_ = true;
}

void Scene::unschedule_visit() {
	is_visit_scheduled_ = false;
}

bool Scene::add(std::shared_ptr<Node> node) {
	return root_node_->add_child(node);
}

void Scene::add(std::shared_ptr<Sound> sound) {
	if (!sound) {
		return;
	}

	sounds_.emplace_back(sound);
}

std::shared_ptr<Node> Scene::root_node() const {
	return root_node_;
}

Camera& Scene::camera() {
	return camera_;
}

// protected
bool Scene::init() {
	return true;
}

// protected
void Scene::update(float dt) {}

// protected
void Scene::visit() {}

// protected
Context const& Scene::ctx() const {
	return ctx_;
}

// private
bool Scene::init_scene() {
	return init();
}

// private
void Scene::update_all(float dt) {
	// auto release sounds
	std::erase_if(sounds_, [](auto const& sound) {
		return sound.use_count() <= 1 && sound->voice_count() == 0;
	});

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