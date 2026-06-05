#include <Context.hh>
#include <raylib.h>

namespace aether {

// private
Context::Context(Window& _window, Renderer& _renderer, SoLoud::Soloud& _soloud, TextureRepository& _texture_repository,
                 AnimationRepository& _animation_repository, AudioRepository& _audio_repository,
                 SceneScheduler& _scene_scheduler)
    : core_services(_window, _renderer, _soloud)
    , resource_services(_texture_repository, _animation_repository, _audio_repository)
    , scene_scheduler(_scene_scheduler)
    , total_time_(0.f)
    , frame_elapsed_(0.f)
    , frame_count_(0)
    , running_fps_(0) {}

// public
Context::~Context() = default;

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