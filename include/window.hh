#pragma once
#include <cstdint>
#include <functional>
#include <math/size.hh>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace aether {

class game;

class window final {
	friend class game;

public:
	~window();
	[[nodiscard]] std::string_view title() const;
	[[nodiscard]] size<int> target_size() const;

private:
	window();
	bool init_(std::string_view title, size<int> _size, int fps);
	void shutdown_();
	void update_();
	[[nodiscard]] bool should_close_() const;
	[[nodiscard]] bool is_minimized_() const;

	std::string title_;
	size<int> target_size_;
};

} // namespace aether