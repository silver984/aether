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

// private
window::window()  = default;
window::~window() = default;

void window::on_resize(std::weak_ptr<std::function<void(window&)>> callback) {
	on_resize_callbacks_.emplace_back(std::move(callback));
}

bool window::should_close() const {
	return WindowShouldClose();
}

bool window::is_minimized() const {
	return IsWindowMinimized();
}

std::string_view window::title() const {
	return title_;
}

size<uint32_t> window::default_size() const {
	return default_size_;
}

// private
bool window::init(std::string_view title, size<int> resolution, int fps) {
#ifdef AETHER_DEBUG
	log::__impl::__create_log_file();

	#ifdef WIN32
	if (!win32::enable_console_colors()) {
		warninglog("Couldn't enable console colors");
	}
	#endif
#endif

	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
	size<int> const minimum_size = size<int>(640, 360);
	title_                       = std::string(title);
	default_size_                = static_cast<size<uint32_t>>(util::max(minimum_size, resolution));
	InitWindow(default_size_.width, default_size_.height, title_.c_str());

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

// private
void window::shutdown() {
	CloseWindow();
}

// private
void window::update() {
	for (auto iterator = on_resize_callbacks_.begin(); iterator != on_resize_callbacks_.end();) {
		auto callback = iterator->lock();

		if (!callback) {
			iterator = on_resize_callbacks_.erase(iterator);
			continue;
		}

		(*callback)(*this);
		++iterator;
	}
}

} // namespace aether