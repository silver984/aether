#pragma once
#include <cstdint>

namespace ae {

class Funkin;
class Window;
class Renderer;
class Resource;
class Director;

class Context final {
	friend class Funkin;
private:
	explicit Context(
		Window* window_ptr,
		Renderer* renderer_ptr,
		Resource* resource_ptr,
		Director* director_ptr
	);

	Context() = delete;

public:
	~Context();

	Context(Context const&) = delete;
	Context(Context&&) = delete;
	Context& operator =(Context const&) = delete;
	Context& operator =(Context&&) = delete;

	[[nodiscard]] Window* window() const;
	[[nodiscard]] Renderer* renderer() const;
	[[nodiscard]] Resource* resource() const;
	[[nodiscard]] Director* director() const;
	[[nodiscard]] float delta_time() const;
	[[nodiscard]] float total_time() const;
	[[nodiscard]] float dpi_scale() const;
	[[nodiscard]] uint32_t running_fps() const;

private:
	Window* window_;
	Renderer* renderer_;
	Resource* resource_;
	Director* director_;
	float total_time_;
	float dpi_scale_;
	uint32_t running_fps_;
};

}