#pragma once
#include <aether/general.h>

struct Texture;

namespace SoLoud {

class Soloud;

}

namespace aether::core {

class window;
class renderer;
class scene_scheduler;

template <typename>
class resources;

} // namespace aether::core

namespace aether {

class game;

class context final {
	friend class game;

public:
	~context() noexcept;
	DELETE_COPY_AND_MOVE(context);

	core::window& window;
	core::renderer& renderer;
	core::scene_scheduler& scene_scheduler;
	core::resources<Texture>& textures;
	SoLoud::Soloud& soloud;

private:
	context(game& game) noexcept;
};

} // namespace aether