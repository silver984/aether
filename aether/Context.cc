#include <Context.hh>
#include <raylib.h>

namespace aether {

// private
Context::Context(Window& window, Renderer& renderer, SoLoud::Soloud& soloud, TextureRepository& textures,
                 AnimationRepository& animations, AudioRepository& audios, SceneScheduler& scene_scheduler)
    : core_(window, renderer, soloud)
    , resource_(textures, animations, audios)
    , scene_scheduler_(scene_scheduler)
    , total_time_(0.f)
    , frame_elapsed_(0.f)
    , frame_count_(0)
    , running_fps_(0) {}

// public
Context::~Context() = default;

CoreServices const& Context::core() const {
	return core_;
}

ResourceServices const& Context::resource() const {
	return resource_;
}

SceneScheduler& Context::scene_scheduler() const {
	return scene_scheduler_;
}

float Context::delta_time() const {
	return GetFrameTime();
}

float Context::total_time() const {
	return total_time_;
}

uint32_t Context::frame_count() const {
	return frame_count_;
}

uint32_t Context::running_fps() const {
	return running_fps_;
}

// private
void Context::update_frame_context() {
	float const dt = delta_time();
	frame_elapsed_ += dt;
	total_time_ += dt;
	frame_count_++;

	while (frame_elapsed_ >= 1.f) {
		running_fps_ = frame_count_;
		frame_count_ = 0;
		frame_elapsed_ -= 1.f;
	}
}

} // namespace aether