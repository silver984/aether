#pragma once
#include <math/size.hh>
#include <memory>
#include <string_view>

namespace aether {

class Context;

class Aether final {
public:
	struct init_descriptor final {
		std::string_view window_title;
		size<int> resolution;
		int fps = 240;
	};

	Aether();
	~Aether();
	Aether(Aether const&)            = delete;
	Aether(Aether&&)                 = delete;
	Aether& operator=(Aether const&) = delete;
	Aether& operator=(Aether&&)      = delete;

	bool init(init_descriptor desc) const;
	void run() const;
	[[nodiscard]] Context const& context() const;

private:
	struct impl;
	std::unique_ptr<impl> impl_;
};

} // namespace aether