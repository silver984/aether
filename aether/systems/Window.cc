#include <aether/debug/log.hh>
#include <aether/systems/Window.hh>
#include <aether/util/math.hh>
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
		// already initialized
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
	using enum ConfigFlags;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);

	title_                              = std::string(title);
	size<int> const minimum_screen_size = {640, 360};
	screen_size_                        = math::max(minimum_screen_size, resolution);
	InitWindow(screen_size_.width, screen_size_.height, title_.c_str());

	if (!IsWindowReady()) {
		errorlog("Not ready");
		return false;
	}

	SetTargetFPS(std::max(1, target_fps));
	SetExitKey(KeyboardKey::KEY_NULL);
	SetWindowMinSize(minimum_screen_size.width, minimum_screen_size.height);

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