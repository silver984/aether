#include <aether/systems/Context.hh>
#include <raylib.h>

namespace ae {

// private
Context::Context(Window& window_ref, Audio& audio_ref, Renderer& renderer_ref, TextureRepo& texture_repo_ref,
                 TextureAtlasRepo& texture_atlas_repo_ref, Director& director_ref)
    : window(window_ref), audio(audio_ref), renderer(renderer_ref), texture_repo(texture_repo_ref),
      texture_atlas_repo(texture_atlas_repo_ref), director(director_ref), total_time_(0.f), frame_elapsed_(0.f),
      frame_count_(0), running_fps_(0) {}

// public
Context::~Context() = default;

float Context::delta_time() const {
	return GetFrameTime();
}

float Context::total_time() const {
	return total_time_;
}

std::uint32_t Context::frame_count() const {
	return frame_count_;
}

std::uint32_t Context::running_fps() const {
	return running_fps_;
}

// private
void Context::update_frame_ctx() {
	float dt = delta_time();
	frame_elapsed_ += dt;
	total_time_ += dt;
	frame_count_++;

	while (frame_elapsed_ >= 1.f) {
		running_fps_ = frame_count_;
		frame_count_ = 0;
		frame_elapsed_ -= 1.f;
	}
}

} // namespace ae