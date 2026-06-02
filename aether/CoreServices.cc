#include <CoreServices.hh>

namespace aether {

// private
CoreServices::CoreServices(Window& _window, AudioManager& _audio_manager, Renderer& _renderer)
    : window(_window)
    , audio_manager(_audio_manager)
    , renderer(_renderer) {}

CoreServices::~CoreServices() = default;

} // namespace aether