#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <Context.hh>
#include <objects/abstract/Scene.hh>
#include <services/SceneScheduler.hh>
#include <utility>

namespace aether {

// private
SceneScheduler::SceneScheduler()  = default;
SceneScheduler::~SceneScheduler() = default;

void SceneScheduler::replace_scene(std::unique_ptr<Scene> new_scene) {
	if (!new_scene) {
#ifdef AETHER_DEBUG
		errorlog("Can't switch to a nullptr scene");
#endif
		return;
	}

	pending_scene_ = std::move(new_scene);

#ifdef AETHER_VERBOSE_DEBUG
	debuglog("Replacing scene");
	tracelog("Pending scene: {}", fmt::ptr(pending_scene_.get()));
#endif
}

// private
void SceneScheduler::cleanup() {
	current_scene_.reset();
	pending_scene_.reset();
}

// private
void SceneScheduler::update_scene(float dt) {
	if (pending_scene_) {
		current_scene_ = std::move(pending_scene_);
#ifdef AETHER_DEBUG
	#ifdef AETHER_VERBOSE_DEBUG
		debuglog("Scene replaced");
	#endif
		infolog("Scene replaced");
#endif
	}

	current_scene_->update_all(dt);
}

// private
void SceneScheduler::draw_scene() {
	current_scene_->draw_all();
}

// private
bool SceneScheduler::has_pending_scene() const {
	return pending_scene_ != nullptr;
}

} // namespace aether