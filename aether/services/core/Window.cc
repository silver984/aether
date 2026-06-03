#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <services/core/Window.hh>
#include <util/math.hh>
#ifdef WIN32
	#include <platforms/win32.hh>
#endif
#include <algorithm>
#include <raylib.h>
#include <utility>

namespace aether {

// private
Window::Window()  = default;
Window::~Window() = default;

void Window::on_resize(std::weak_ptr<std::function<void(Window&)>> callback) {
	on_resize_callbacks_.emplace_back(std::move(callback));
}

bool Window::should_close() const {
	return WindowShouldClose();
}

bool Window::is_minimized() const {
	return IsWindowMinimized();
}

std::string_view Window::title() const {
	return title_;
}

size<std::uint32_t> Window::default_size() const {
	return default_size_;
}

// private
bool Window::init(init_descriptor desc) {
#ifdef AETHER_DEBUG
	log::impl::create_log_file();

	#ifdef WIN32
	if (!win32::enable_console_colors()) {
		warninglog("Couldn't enable console colors");
	}
	#endif
#endif

	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
	size<int> const minimum_size = size<int>(640, 360);
	title_                       = std::string(desc.title);
	default_size_                = static_cast<size<std::uint32_t>>(util::max(minimum_size, desc.resolution));
	InitWindow(default_size_.width, default_size_.height, title_.c_str());

	if (!IsWindowReady()) {
#ifdef AETHER_DEBUG
		errorlog("Not ready");
#endif
		return false;
	}

	SetTargetFPS(std::max(1, desc.fps));
	SetExitKey(KEY_NULL);
	SetWindowMinSize(minimum_size.width, minimum_size.height);

#ifdef AETHER_VERBOSE_DEBUG
	debuglog("Initialized");
#endif
	return true;
}

// private
void Window::shutdown() {
	CloseWindow();
}

// private
void Window::update() {
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