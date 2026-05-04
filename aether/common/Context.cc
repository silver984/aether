#include <aether/common/Context.hh>
#include <aether/internal/rl.hh>

namespace ae {

// private
Context::Context(Window* window_ptr, Renderer* renderer_ptr, Resource* resource_ptr) :
	window_(window_ptr),
	renderer_(renderer_ptr),
	resource_(resource_ptr),
	total_time_(0.f),
	dpi_scale_(1.f),
	running_fps_(0)
{}

// private
Context::~Context() = default;

Window* Context::window() const {
	return window_;
}

Renderer* Context::renderer() const {
	return renderer_;
}

Resource* Context::resource() const {
	return resource_;
}

float Context::delta_time() const {
	return rl::delta_time();
}

float Context::total_time() const {
	return total_time_;
}

float Context::dpi_scale() const {
	return dpi_scale_;
}

uint32_t Context::running_fps() const {
	return running_fps_;
}

}