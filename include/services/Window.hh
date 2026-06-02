#pragma once
#include <cstdint>
#include <math/size.hh>
#include <string>
#include <string_view>
#include <vector>

namespace aether {

class Aether;

class Window final {
	friend class Aether;

private:
	Window();

public:
	struct init_descriptor final {
		std::string_view title;
		size<int> resolution;
		int fps;
	};

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
	bool init(init_descriptor desc);
	void shutdown();

	std::string title_;
	size<int> screen_size_;
	bool is_initialized_;
};

} // namespace ae