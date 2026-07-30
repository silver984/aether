#pragma once
#include <math/size.hh>
#include <string>
#include <string_view>

namespace aether {

class game;

class window final {
	friend class game;

public:
	~window();
	[[nodiscard]] std::string_view title() const;
	[[nodiscard]] size<int> target_size() const;
	[[nodiscard]] int target_fps() const;

private:
	window();
	bool init_(std::string_view title, size<int> bounds, int fps);
	void shutdown_();
	void update_();
	[[nodiscard]] bool should_close_() const;
	[[nodiscard]] bool is_minimized_() const;

	std::string title_;
	size<int> target_size_;
	int target_fps_;
};

} // namespace aether