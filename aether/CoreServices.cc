#include <CoreServices.hh>

namespace aether {

// private
CoreServices::CoreServices(Window& _window, Renderer& _renderer, SoLoud::Soloud& _soloud)
    : window(_window)
    , renderer(_renderer)
    , soloud(_soloud) {}

CoreServices::~CoreServices() = default;

} // namespace aether