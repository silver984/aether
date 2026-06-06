#include <CoreServices.hh>

namespace aether {

// private
CoreServices::CoreServices(Window& window, Renderer& renderer, SoLoud::Soloud& soloud)
    : window_(window)
    , renderer_(renderer)
    , soloud_(soloud) {}

CoreServices::~CoreServices() = default;

Window& CoreServices::window() const {
	return window_;
}

Renderer& CoreServices::renderer() const {
	return renderer_;
}

SoLoud::Soloud& CoreServices::soloud() const {
	return soloud_;
}

} // namespace aether