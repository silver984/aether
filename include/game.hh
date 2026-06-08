#pragma once
#include <math/size.hh>
#include <memory>
#include <string_view>

namespace aether {

class context;

class game final {
    public:
	struct init_descriptor final {
		std::string_view window_title;
		size<int> resolution;
		int fps = 240;
	};

	game();
	~game();
	game(game const&)            = delete;
	game(game&&)                 = delete;
	game& operator=(game const&) = delete;
	game& operator=(game&&)      = delete;

	bool init(init_descriptor const& desc) const;
	void run() const;
	[[nodiscard]] context const& fetch_context() const;

    private:
	struct impl;
	std::unique_ptr<impl> impl_;
};

} // namespace aether