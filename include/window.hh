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

	void on_resize(std::weak_ptr<std::function<void(window&)>> callback);
	[[nodiscard]] bool should_close() const;
	[[nodiscard]] bool is_minimized() const;
	[[nodiscard]] std::string_view title() const;
	[[nodiscard]] size<uint32_t> default_size() const;

    private:
	bool init(std::string_view title, size<int> resolution, int fps);
	void shutdown();
	void update();

	std::vector<std::weak_ptr<std::function<void(window&)>>> on_resize_callbacks_;
	std::string title_;
	size<uint32_t> default_size_;
};

} // namespace aether