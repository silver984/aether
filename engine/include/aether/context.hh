#pragma once
#include <aether/general.h>

struct Texture;

namespace SoLoud {

class Soloud;

}

namespace aether {

class game;
class window;
class renderer;
class scene_scheduler;
class zip_archive;

template <typename>
class resources;

class context final {
	friend class game;

public:
	~context() noexcept;
	DELETE_COPY_AND_MOVE(context);

	[[nodiscard]] inline SoLoud::Soloud& soloud() const noexcept {
		return soloud_;
	}

	window& window;
	renderer& renderer;
	scene_scheduler& scene_scheduler;
	resources<Texture>& textures;

private:
	context(game& game) noexcept;
	SoLoud::Soloud& soloud_;
};

} // namespace aether