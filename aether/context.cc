#include <context.hh>

namespace aether {

context::context(descriptor const& desc)
        : window_(desc.rwindow)
        , renderer_(desc.rrenderer)
        , textures_(desc.rtextures)
        , animations_(desc.ranimations)
        , audios_(desc.raudios)
        , scene_scheduler_(desc.rscene_scheduler)
        , soloud_(desc.rsoloud) {}

context::~context() = default;

window& context::get_window() const {
	return window_;
}

renderer& context::get_renderer() const {
	return renderer_;
}

texture_repository& context::textures() const {
	return textures_;
}

animation_repository& context::animations() const {
	return animations_;
}

audio_repository& context::audios() const {
	return audios_;
}

scene_scheduler& context::get_scene_scheduler() const {
	return scene_scheduler_;
}

SoLoud::Soloud& context::soloud() const {
	return soloud_;
}

} // namespace aether