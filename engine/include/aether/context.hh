#pragma once

struct Texture;

namespace SoLoud {

class Soloud;

}

namespace aether {

class game;
class window;
class renderer;
class scene_scheduler;
template <typename>
class resources;
class ziparc;

// class animation_repository;
// class audio_repository;

class context final {
	friend class game;

public:
	~context() noexcept;
	[[nodiscard]] window& window() const noexcept;
	[[nodiscard]] renderer& renderer() const noexcept;
	[[nodiscard]] SoLoud::Soloud& soloud() const noexcept;
	[[nodiscard]] scene_scheduler& scene_scheduler() const noexcept;
	[[nodiscard]] resources<Texture>& textures() const noexcept;
	[[nodiscard]] ziparc& aether_resources() const noexcept;

	// [[nodiscard]] animation_repository& animations() const;
	// [[nodiscard]] audio_repository& audios() const;

private:
	context(game& game) noexcept;
	game& game_;
};

} // namespace aether