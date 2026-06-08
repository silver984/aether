#pragma once

namespace SoLoud {

class Soloud;

}

namespace aether {

class context;
class window;
class renderer;

class core_services final {
	friend class context;

    private:
	core_services(window& window, renderer& renderer, SoLoud::Soloud& soloud);

    public:
	~core_services();

	[[nodiscard]] window& fetch_window() const;
	[[nodiscard]] renderer& fetch_renderer() const;
	[[nodiscard]] SoLoud::Soloud& soloud() const;

    private:
	window& window_;
	renderer& renderer_;
	SoLoud::Soloud& soloud_;
};

} // namespace aether