#include <aether/context.hh>
#include <aether/game.hh>

namespace aether {

context::context(game& game) noexcept
        : window(game.window_)
        , renderer(game.renderer_)
        , scene_scheduler(game.scene_scheduler_)
        , textures(game.textures_)
        , soloud(game.soloud_) {
}

context::~context() noexcept = default;

} // namespace aether