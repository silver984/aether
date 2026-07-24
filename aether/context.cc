#include <context.hh>

namespace aether {

context::context(descriptor const& desc)
        : desc_(desc) {
}
context::~context() = default;

window& context::get_window() const {
	return desc_.window;
}

renderer& context::get_renderer() const {
	return desc_.renderer;
}

texture_cache& context::textures() const {
	return desc_.textures;
}

animation_repository& context::animations() const {
	return desc_.animations;
}

audio_repository& context::audios() const {
	return desc_.audios;
}

scene_scheduler& context::get_scene_scheduler() const {
	return desc_.scene_scheduler;
}

SoLoud::Soloud& context::soloud() const {
	return desc_.soloud;
}

} // namespace aether