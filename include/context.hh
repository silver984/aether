#pragma once

namespace SoLoud {

class Soloud;

}

namespace aether {

class game;
class window;
class renderer;
class texture_cache;
class animation_repository;
class audio_repository;
class scene_scheduler;

class context final {
	friend class game;

public:
	struct descriptor final {
		window& rwindow;
		renderer& rrenderer;
		texture_cache& rtextures;
		animation_repository& ranimations;
		audio_repository& raudios;
		scene_scheduler& rscene_scheduler;
		SoLoud::Soloud& rsoloud;
	};

	~context();
	[[nodiscard]] window& get_window() const;
	[[nodiscard]] renderer& get_renderer() const;
	[[nodiscard]] texture_cache& textures() const;
	[[nodiscard]] animation_repository& animations() const;
	[[nodiscard]] audio_repository& audios() const;
	[[nodiscard]] scene_scheduler& get_scene_scheduler() const;
	[[nodiscard]] SoLoud::Soloud& soloud() const;

private:
	context(descriptor const& desc);

	window& window_;
	renderer& renderer_;
	texture_cache& textures_;
	animation_repository& animations_;
	audio_repository& audios_;
	scene_scheduler& scene_scheduler_;
	SoLoud::Soloud& soloud_;
};

} // namespace aether