#include <aether/log.hh>
#include <aether/math.hh>
#include <aether/window.hh>
#include <algorithm>
#include <raylib.h>

namespace aether::core {

window::window()
        : target_fps_(0) {
}

window::~window() = default;

std::string_view window::title() const {
	return title_;
}

size<int> window::target_size() const {
	return target_size_;
}

int window::target_fps() const {
	return target_fps_;
}

bool window::init_(std::string_view title, size<int> bounds, int fps) {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
	size<int> const minimum_size = size<int>(640, 360);
	title_                       = std::string(title);
	target_size_                 = max(minimum_size, bounds);
	target_fps_                  = std::max(1, fps);
	InitWindow(target_size_.width, target_size_.height, title_.c_str());

	if (!IsWindowReady()) {
		AETHER_ENGINE_ERRORLOG("Not ready");
		return false;
	}

	SetTargetFPS(0);
	SetExitKey(KEY_NULL);
	SetWindowMinSize(minimum_size.width, minimum_size.height);

	AETHER_ENGINE_INFOLOG("Initialized");
	return true;
}

void window::shutdown_() {
	CloseWindow();
}

void window::update_() {
	if (IsKeyPressed(KEY_F11)) {
		ToggleBorderlessWindowed();
	}
}

bool window::should_close_() const {
	return WindowShouldClose();
}

bool window::is_minimized_() const {
	return IsWindowMinimized();
}

} // namespace aether::core