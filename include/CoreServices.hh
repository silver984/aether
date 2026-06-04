#pragma once

namespace SoLoud {

class Soloud;

}

namespace aether {

class Context;
class Window;
class Renderer;

class CoreServices final {
	friend class Context;

private:
	CoreServices(Window& _window, Renderer& _renderer, SoLoud::Soloud& _soloud);

public:
	~CoreServices();
	CoreServices(CoreServices const&)            = delete;
	CoreServices(CoreServices&&)                 = delete;
	CoreServices& operator=(CoreServices const&) = delete;
	CoreServices& operator=(CoreServices&&)      = delete;

	Window& window;
	Renderer& renderer;
	SoLoud::Soloud& soloud;
};

} // namespace aether