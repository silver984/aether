#ifdef AETHER_DEBUG
	#include <debug/log.hh>
#endif
#include <context.hh>
#include <scene.hh>
#include <scene_scheduler.hh>
#include <utility>

namespace aether {

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

void scene_scheduler::cleanup_() {
	current_scene_.reset();
	pending_scene_.reset();
}

void scene_scheduler::update_scene_(float dt) {
	if (pending_scene_) {
		current_scene_ = std::move(pending_scene_);
#ifdef AETHER_DEBUG
	#ifdef AETHER_VERBOSE_DEBUG
		debuglog("Scene replaced");
	#endif
		infolog("Scene replaced");
#endif
	}

	current_scene_->update_all_(dt);
}

void scene_scheduler::draw_scene_() {
	current_scene_->draw_all_();
}

bool scene_scheduler::has_pending_scene_() const {
	return pending_scene_ != nullptr;
}

} // namespace aether