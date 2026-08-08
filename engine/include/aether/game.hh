#pragma once
// #include <aether/animation_repository.hh>
// #include <audio_repository.hh>
#include <aether/context.hh>
#include <aether/general.h>
// #include <lua/manager.hh>
#include <aether/renderer.hh>
#include <aether/resources.hh>
#include <aether/scene_scheduler.hh>
#include <aether/size.hh>
#include <aether/window.hh>
#include <aether/zip_archive.hh>
#include <soloud.h>
#include <string_view>

namespace aether {

struct game_init_args final {
	std::string_view window_title;
	size<int> resolution;
	int fps = 240;
};

class game final {
	friend class context;

public:
	game() noexcept;
	~game() noexcept;
	DELETE_COPY_AND_MOVE(game);

	bool init(game_init_args const& args);
	void run();

	context const ctx;

private:
	void shutdown_();

	core::window window_;
	core::renderer renderer_;
	core::scene_scheduler scene_scheduler_;
	core::resources<Texture> textures_;
	SoLoud::Soloud soloud_;
	bool is_initialized_;
};

} // namespace aether