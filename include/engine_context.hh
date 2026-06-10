#pragma once

namespace aether {

class game;
class window;

class engine_context final {
	friend class game;

public:
	~engine_context();

private:
	struct descriptor final {
		window& window_ref;
	};

	engine_context(descriptor const& desc);

	window& window_;
};

} // namespace aether