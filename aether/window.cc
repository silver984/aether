#ifdef AETHER_DEBUG
	#include <debug/log.hh>
#endif
#include <util/math.hh>
#include <window.hh>
#ifdef WIN32
	#include <debug/win32.hh>
#endif
#include <algorithm>
#include <raylib.h>
#include <utility>

namespace aether {

window::window()  = default;
window::~window() = default;

std::string_view window::title() const {
	return title_;
}

size<int> window::target_size() const {
	return target_size_;
}

bool window::init_(std::string_view title, size<int> resolution, int fps) {
#ifdef AETHER_DEBUG
	log::impl_::create_log_file_();

	#ifdef WIN32
	if (!win32_::enable_console_colors_()) {
		warninglog("Couldn't enable console colors");
	}
	#endif
#endif

	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
	size<int> const minimum_size = size<int>(640, 360);
	title_                       = std::string(title);
	target_size_                 = util::max(minimum_size, resolution);
	InitWindow(target_size_.width, target_size_.height, title_.c_str());

	if (!IsWindowReady()) {
#ifdef AETHER_DEBUG
		errorlog("Not ready");
#endif
		return false;
	}

	SetTargetFPS(std::max(1, fps));
	SetExitKey(KEY_NULL);
	SetWindowMinSize(minimum_size.width, minimum_size.height);

#ifdef AETHER_VERBOSE_DEBUG
	debuglog("Initialized");
#endif
	return true;
}

void window::shutdown_() {
	CloseWindow();
#ifdef AETHER_DEBUG
	log::impl_::close_log_file_();
#endif
}

bool window::should_close_() const {
	return WindowShouldClose();
}

bool window::is_minimized_() const {
	return IsWindowMinimized();
}

} // namespace aether