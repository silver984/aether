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
	CoreServices(Window& window, Renderer& renderer, SoLoud::Soloud& soloud);

public:
	~CoreServices();

	[[nodiscard]] Window& window() const;
	[[nodiscard]] Renderer& renderer() const;
	[[nodiscard]] SoLoud::Soloud& soloud() const;

private:
	Window& window_;
	Renderer& renderer_;
	SoLoud::Soloud& soloud_;
};

} // namespace aether