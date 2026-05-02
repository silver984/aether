#include <aether/common/Context.hh>
#include <aether/internal/rl.hh>

namespace ae {

// private
Context::Context(Window* window_ptr) :
	window_(window_ptr),
	total_time_(0.f),
	running_fps_(0)
{}

// private
Context::~Context() = default;

Window const* Context::window() const {
	return window_;
}

float Context::delta_time() const {
	return rl::delta_time();
}

float Context::total_time() const {
	return total_time_;
}

uint32_t Context::running_fps() const {
	return running_fps_;
}

}