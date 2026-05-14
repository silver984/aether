#ifndef __AETHER_SYSTEMS_WINDOW_HH__
#define __AETHER_SYSTEMS_WINDOW_HH__

#include <aether/math/size.hh>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace ae {

class Funkin;

class Window final {
	friend class Funkin;

  public:
	Window();
	~Window();
	Window(Window const&)            = delete;
	Window(Window&&)                 = delete;
	Window& operator=(Window const&) = delete;
	Window& operator=(Window&&)      = delete;

	[[nodiscard]] bool should_close() const;
	[[nodiscard]] bool is_minimized() const;
	[[nodiscard]] bool was_resized() const;
	[[nodiscard]] std::string_view title() const;
	[[nodiscard]] size<int> screen_size() const;

  private:
	bool init(std::string_view title, size<int> resolution, int target_fps);
	void shutdown();

	std::string title_;
	size<int> screen_size_;
	bool is_initialized_;
};

} // namespace ae

#endif