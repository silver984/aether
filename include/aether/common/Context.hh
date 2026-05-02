#pragma once
#include <cstdint>

namespace ae {

class Funkin;
class Window;

class Context final {
	friend class Funkin;
private:
	explicit Context(Window* window_ptr);
	~Context();

public:
	Context(Context const&) = delete;
	Context(Context&&) = delete;
	Context& operator =(Context const&) = delete;
	Context& operator =(Context&&) = delete;

	[[nodiscard]] Window const* window() const;
	[[nodiscard]] float delta_time() const;
	[[nodiscard]] float total_time() const;
	[[nodiscard]] uint32_t running_fps() const;

private:
	Window* window_;
	float total_time_;
	uint32_t running_fps_;
};

}