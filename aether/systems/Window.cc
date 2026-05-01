#include <aether/systems/Window.hh>
#include <aether/internal/rl.hh>
#include <raylib.h>
#include <algorithm>

namespace ae {

// private
Window::Window() :
	target_fps_(0),
	is_initialized_(false)
{}

// private
Window::~Window() = default;

bool Window::is_initialized() const {
	return is_initialized_;
}

bool Window::should_close() const {
	return rl::should_window_close();
}

// private
bool Window::init(std::string_view title, size<int> const& resolution, int target_fps) {
	if (is_initialized_) {
		return true;
	}

	title_ = std::string(title);
	target_fps_ = std::max(1, target_fps);
	// TODO: clamp resolution

	if (rl::init_window(title_.c_str(), resolution, target_fps_)) {
		is_initialized_ = true;
		return true;
	}

	return false;
}

// private
void Window::shutdown() {
	CloseWindow();
	is_initialized_ = false;
}

}