#include <aether/log.hh>
#include <aether/scene.hh>
#include <aether/scene_scheduler.hh>
#include <utility>

namespace aether {

scene_scheduler::scene_scheduler() {
	this->expose_();
}

scene_scheduler::~scene_scheduler() noexcept = default;

void scene_scheduler::replace_scene(unique_ref<scene> new_scene) {
	if (!new_scene) {
		AETHER_ENGINE_ERRORLOG("Can't switch to a nullptr scene");
		return;
	}
	pending_scene_ = std::move(new_scene);
	AETHER_ENGINE_DEBUGLOG("Replacing scene");
	AETHER_ENGINE_TRACELOG("Pending scene: {}", fmt::ptr(pending_scene_.get()));
}

void scene_scheduler::cleanup_() {
	current_scene_.release();
	pending_scene_.release();
}

void scene_scheduler::update_scene_(float dt) {
	if (pending_scene_) {
		current_scene_ = std::move(pending_scene_);
		AETHER_ENGINE_INFOLOG("Scene replaced");
	}
	if (current_scene_) {
		current_scene_->update_all_(dt);
	}
}

void scene_scheduler::draw_scene_() {
	if (current_scene_) {
		current_scene_->draw_all_();
	}
}

bool scene_scheduler::has_pending_scene_() const {
	return pending_scene_ != nullptr;
}

} // namespace aether