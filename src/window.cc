#include <aether/log.hh>
#include <aether/math.hh>
#include <aether/window.hh>

#include <raylib.h>

namespace aether::_window_impl {

size<uint32_t> bounds_;
uint32_t fps_{0};

bool init_(std::string_view title, size<uint32_t> bounds, uint32_t fps) {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);

	bounds_ = bounds;
	fps_    = fps;

	InitWindow(bounds.width, bounds.height, title.data());

	if (!IsWindowReady()) {
		return false;
	}

	SetTargetFPS(0);
	SetExitKey(KEY_NULL);
	SetWindowMinSize(640, 360);

	return true;
}

void close_() { CloseWindow(); }
bool should_close_() { return WindowShouldClose(); }
bool is_minimized_() { return IsWindowMinimized(); }

} // namespace aether::_window_impl

namespace aether::window {

size<uint32_t> bounds() { return _window_impl::bounds_; }
uint32_t fps() { return _window_impl::fps_; }

} // namespace aether::window