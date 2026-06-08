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

private:
	window();

public:
	~window();
	window(window const&)            = delete;
	window(window&&)                 = delete;
	window& operator=(window const&) = delete;
	window& operator=(window&&)      = delete;

	[[nodiscard]] std::string_view title() const;
	[[nodiscard]] size<int> target_size() const;

private:
	bool init(std::string_view title, size<int> _size, int fps);
	void shutdown();
	[[nodiscard]] bool should_close() const;
	[[nodiscard]] bool is_minimized() const;

	std::string title_;
	size<int> target_size_;
};

} // namespace aether