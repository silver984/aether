#pragma once

namespace aether {

class Context;
class Window;
class AudioManager;
class Renderer;

class CoreServices final {
	friend class Context;

private:
	CoreServices(Window& _window, AudioManager& _audio_manager, Renderer& _renderer);

public:
	~CoreServices();
	CoreServices(CoreServices const&)            = delete;
	CoreServices(CoreServices&&)                 = delete;
	CoreServices& operator=(CoreServices const&) = delete;
	CoreServices& operator=(CoreServices&&)      = delete;

	Window& window;
	AudioManager& audio_manager;
	Renderer& renderer;
};

}