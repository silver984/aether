#include <algorithm>
#include <raylib.h>
#include <util/math.hh>
#include <utility>
#include <window.hh>

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
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
	size<int> const minimum_size = size<int>(640, 360);
	title_                       = std::string(title);
	target_size_                 = util::max(minimum_size, resolution);
	InitWindow(target_size_.width, target_size_.height, title_.c_str());

	if (!IsWindowReady()) {
		return false;
	}

	SetTargetFPS(std::max(1, fps));
	SetExitKey(KEY_NULL);
	SetWindowMinSize(minimum_size.width, minimum_size.height);

	return true;
}

void window::shutdown_() {
	CloseWindow();
}

bool window::should_close_() const {
	return WindowShouldClose();
}

bool window::is_minimized_() const {
	return IsWindowMinimized();
}

} // namespace aether