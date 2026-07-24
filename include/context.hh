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
		window& window;
		renderer& renderer;
		texture_cache& textures;
		animation_repository& animations;
		audio_repository& audios;
		scene_scheduler& scene_scheduler;
		SoLoud::Soloud& soloud;
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

	descriptor const desc_;
};

} // namespace aether