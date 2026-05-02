#pragma once
#include <cstdint>

namespace ae {

class Funkin;
class Window;
class Renderer;

class Context final {
	friend class Funkin;
private:
	explicit Context(Window* window_ptr, Renderer* renderer_ptr);
	~Context();

public:
	Context(Context const&) = delete;
	Context(Context&&) = delete;
	Context& operator =(Context const&) = delete;
	Context& operator =(Context&&) = delete;

	[[nodiscard]] Window const* window() const;
	[[nodiscard]] Renderer const* renderer() const;
	[[nodiscard]] float delta_time() const;
	[[nodiscard]] float total_time() const;
	[[nodiscard]] float dpi_scale() const;
	[[nodiscard]] uint32_t running_fps() const;

private:
	Window* window_;
	Renderer* renderer_;
	float total_time_;
	float dpi_scale_;
	uint32_t running_fps_;
};

}