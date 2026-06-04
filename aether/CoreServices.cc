#include <CoreServices.hh>

namespace aether {

// private
CoreServices::CoreServices(Window& _window, Renderer& _renderer)
    : window(_window)
    , renderer(_renderer) {}

CoreServices::~CoreServices() = default;

} // namespace aether