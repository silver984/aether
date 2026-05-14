#include <aether/common/log.hh>
#include <aether/math/util.hh>
#include <aether/systems/Window.hh>
#ifdef WIN32
#include <aether/platforms/win32.hh>
#endif
#include <algorithm>
#include <raylib.h>

namespace ae {

// private
Window::Window() : is_initialized_(false) {}

// private
Window::~Window() = default;

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
bool Window::init(std::string_view title, size<int> resolution, int target_fps) {
	if (is_initialized_) {
		return true;
	}

#ifdef AETHER_DEBUG
	log::impl::create_log_file();
#ifdef WIN32
	if (!win32::enable_console_colors()) {
		warninglog("Couldn't enable console colors");
	}
#endif
#endif

	SetTraceLogCallback([](int, char const*, va_list) {});

	SetConfigFlags(ConfigFlags::FLAG_WINDOW_RESIZABLE |
				   ConfigFlags::FLAG_WINDOW_ALWAYS_RUN |
				   ConfigFlags::FLAG_WINDOW_TRANSPARENT);

	title_ = std::string(title);
	screen_size_ = math::max(resolution / 2, resolution);

	InitWindow(screen_size_.width, screen_size_.height, title_.c_str());

	if (!IsWindowReady()) {
		errorlog("Not ready");
		return false;
	}

	SetTargetFPS(std::max(1, target_fps));
	SetExitKey(KeyboardKey::KEY_NULL);

	infolog("Initialized");

	return is_initialized_ = true;
}

// private
void Window::shutdown() {
	CloseWindow();

	debuglog("Closed window");

	is_initialized_ = false;
}

} // namespace ae