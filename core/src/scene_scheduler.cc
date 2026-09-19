#include <aether/log.hh>
#include <aether/scene.hh>
#include <aether/scene_scheduler.hh>

#include <utility>

namespace aether {

void scene_scheduler::replace_scene(unique_ref<scene> new_scene) {
	if (!new_scene) {
		log<error>({"Can't switch to a nullptr scene"});
		return;
	}
	pending_scene_ = std::move(new_scene);
	log<debug>({"Replacing scene"});
	log<trace>({"Pending scene: 0x{:X}", reinterpret_cast<uintptr_t>(pending_scene_.get())});
}

void scene_scheduler::cleanup_() {
	current_scene_.release();
	pending_scene_.release();
}

void scene_scheduler::update_scene_(float dt) {
	if (pending_scene_) {
		current_scene_ = std::move(pending_scene_);
		log<info>({"Scene replaced"});
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

} // namespace aether