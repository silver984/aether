#include <aether/systems/Window.hh>
#include <aether/math/util.hh>
#include <raylib.h>
#include <algorithm>
#include <fmt/format.h>

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

bool Window::was_resized() const {
	return IsWindowResized();
}

std::string_view Window::title() const {
	return title_;
}

size<int> Window::screen_size() const {
	return screen_size_;
}

// private
bool Window::init(std::string_view title, size<int> const& resolution, int target_fps) {
	if (is_initialized_) {
		return true;
	}

	title_ = std::string(title);
	screen_size_ = math::max(size<int>(1, 1), resolution);

	SetTraceLogCallback([](int, char const*, va_list) {});

	SetConfigFlags(
		ConfigFlags::FLAG_WINDOW_RESIZABLE |
		ConfigFlags::FLAG_WINDOW_ALWAYS_RUN |
		ConfigFlags::FLAG_WINDOW_TRANSPARENT
	);

	InitWindow(resolution.width, resolution.height, title_.c_str());

	if (!IsWindowReady()) {
		// TODO: log error
		return false;
	}

	SetTargetFPS(target_fps);
	SetExitKey(KeyboardKey::KEY_NULL);

	return true;
}

// private
void Window::shutdown() {
	CloseWindow();
	is_initialized_ = false;
}

}