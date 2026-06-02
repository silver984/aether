#pragma once
#include <cstdint>
#include <functional>
#include <math/size.hh>
#include <memory>
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

	void on_resize(std::weak_ptr<std::function<void(Window&)>> callback);
	[[nodiscard]] bool should_close() const;
	[[nodiscard]] bool is_minimized() const;
	[[nodiscard]] std::string_view title() const;
	[[nodiscard]] size<std::uint32_t> default_size() const;

private:
	bool init(init_descriptor desc);
	void shutdown();
	void update();

	std::vector<std::weak_ptr<std::function<void(Window&)>>> on_resize_callbacks_;
	std::string title_;
	size<std::uint32_t> default_size_;
	bool is_initialized_;
};

} // namespace aether