#include <aether/context.hh>
#include <aether/game.hh>

namespace aether {

context::context(game& game) noexcept
        : game_(game) {
}
context::~context() noexcept = default;

window& context::window() const noexcept {
	return game_.window_;
}

renderer& context::renderer() const noexcept {
	return game_.renderer_;
}

SoLoud::Soloud& context::soloud() const noexcept {
	return game_.soloud_;
}

scene_scheduler& context::scene_scheduler() const noexcept {
	return game_.scene_scheduler_;
}

resources<Texture>& context::textures() const noexcept {
	return game_.textures_;
}

ziparc& context::aether_resources() const noexcept {
	return game_.aether_resources_;
}

// animation_repository& context::animations() const noexcept {
// 	return game_.animations;
// }

// audio_repository& context::audios() const noexcept {
// 	return game_.audios;
// }

} // namespace aether