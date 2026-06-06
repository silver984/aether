#pragma once
#include <math/size.hh>
#include <memory>
#include <string_view>

namespace aether {

class context;

class app final {
public:
	struct init_descriptor final {
		std::string_view window_title;
		size<int> resolution;
		int fps = 240;
	};

	app();
	~app();
	app(app const&)            = delete;
	app(app&&)                 = delete;
	app& operator=(app const&) = delete;
	app& operator=(app&&)      = delete;

	bool init(init_descriptor desc) const;
	void run() const;
	[[nodiscard]] context const& fetch_context() const;

private:
	struct impl;
	std::unique_ptr<impl> impl_;
};

} // namespace aether