#include <context.hh>
#include <raylib.h>

namespace aether {

// private
context::context(window& _window, renderer& _renderer, SoLoud::Soloud& soloud, texture_repository& textures,
                 animation_repository& animations, audio_repository& audios, scene_scheduler& _scene_scheduler)
        : core_(_window, _renderer, soloud)
        , resource_(textures, animations, audios)
        , scene_scheduler_(_scene_scheduler) {}

// public
context::~context() = default;

core_services const& context::core() const {
	return core_;
}

resource_services const& context::resource() const {
	return resource_;
}

scene_scheduler& context::fetch_scene_scheduler() const {
	return scene_scheduler_;
}

// // private
// void context::update_frame_context() {
// 	frame_elapsed_ += delta_time();
// 	frame_count_++;
// 	while (frame_elapsed_ >= 1.f) {
// 		running_fps_ = frame_count_;
// 		frame_count_ = 0;
// 		frame_elapsed_ -= 1.f;
// 	}
// }

} // namespace aether