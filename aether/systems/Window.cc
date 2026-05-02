#include <aether/systems/Window.hh>
#include <aether/internal/rl.hh>
#include <raylib.h>
#include <algorithm>

namespace ae {

// private
Window::Window() :
	is_initialized_(false)
{}

// private
Window::~Window() = default;

bool Window::is_initialized() const {
	return is_initialized_;
}

bool Window::should_close() const {
	return WindowShouldClose();
}

bool Window::is_minimized() const {
	return IsWindowMinimized();
}

bool Window::is_resized() const {
	return IsWindowResized();
}

std::string_view Window::title() const {
	return title_;
}

// private
bool Window::init(std::string_view title, size<int> const& resolution, int target_fps) {
	if (is_initialized_) {
		return true;
	}

	title_ = std::string(title);
	// TODO: clamp resolution

	if (rl::init_window(title_.c_str(), resolution, std::max(1, target_fps))) {
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