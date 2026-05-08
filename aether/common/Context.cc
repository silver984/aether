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

std::weak_ptr<Window> Context::window() const {
	return window_;
}

std::weak_ptr<Renderer> Context::renderer() const {
	return renderer_;
}

std::weak_ptr<Resource> Context::resource() const {
	return resource_;
}

std::weak_ptr<Director> Context::director() const {
	return director_;
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
	std::weak_ptr<Window> window_ptr,
	std::weak_ptr<Renderer> renderer_ptr,
	std::weak_ptr<Resource> resource_ptr,
	std::weak_ptr<Director> director_ptr
) {
	window_ = std::move(window_ptr);
	renderer_ = std::move(renderer_ptr);
	resource_ = std::move(resource_ptr);
	director_ = std::move(director_ptr);
}

}