#ifdef AETHER_DEBUG
	#include <debug/log.hh>
#endif
#include <context.hh>
#include <scene.hh>
#include <scene_scheduler.hh>
#include <utility>

namespace aether {

// private
scene_scheduler::scene_scheduler()  = default;
scene_scheduler::~scene_scheduler() = default;

void scene_scheduler::replace_scene(std::unique_ptr<scene> new_scene) {
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
void scene_scheduler::cleanup() {
	current_scene_.reset();
	pending_scene_.reset();
}

// private
void scene_scheduler::update_scene(float dt) {
	if (pending_scene_) {
		current_scene_ = std::move(pending_scene_);
#ifdef AETHER_DEBUG
	#ifdef AETHER_VERBOSE_DEBUG
		debuglog("scene replaced");
	#endif
		infolog("scene replaced");
#endif
	}

	current_scene_->update_all(dt);
}

// private
void scene_scheduler::draw_scene() {
	current_scene_->draw_all();
}

// private
bool scene_scheduler::has_pending_scene() const {
	return pending_scene_ != nullptr;
}

} // namespace aether