#include <aether/common/Context.hh>
#include <raylib.h>

namespace ae {

// private
Context::Context() :
	total_time_(0.f),
	running_fps_(0)
{}

// private
Context::~Context() = default;

std::weak_ptr<Window> Context::window_wref() const {
	return window_wref_;
}

std::weak_ptr<Renderer> Context::renderer_wref() const {
	return renderer_wref_;
}

std::weak_ptr<Resource> Context::resource_wref() const {
	return resource_wref_;
}

std::weak_ptr<Director> Context::director_wref() const {
	return director_wref_;
}

float Context::delta_time() const {
	return GetFrameTime();
}

float Context::total_time() const {
	return total_time_;
}

uint32_t Context::running_fps() const {
	return running_fps_;
}

// private
void Context::store_refs(
	std::weak_ptr<Window> window,
	std::weak_ptr<Renderer> renderer,
	std::weak_ptr<Resource> resource,
	std::weak_ptr<Director> director
) {
	window_wref_ = std::move(window);
	renderer_wref_ = std::move(renderer);
	resource_wref_ = std::move(resource);
	director_wref_ = std::move(director);
}

// private
void Context::update_frame_ctx() {
	static uint32_t frame_count = 0;
	static float elapsed = 0.f;

	float dt = delta_time();
	elapsed += dt;
	total_time_ += dt;
	frame_count++;

	while (elapsed >= 1.f) {
		running_fps_ = frame_count;
		frame_count = 0;
		elapsed -= 1.f;
	}
}

}