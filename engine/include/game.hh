#pragma once
// #include <animation_repository.hh>
// #include <audio_repository.hh>
#include <context.hh>
// #include <lua/manager.hh>
#include <math/size.hh>
#include <renderer.hh>
#include <resources.hh>
#include <scene_scheduler.hh>
#include <soloud.h>
#include <string_view>
#include <util/ziparc.hh>
#include <window.hh>

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
	game(game const&) = delete;
	game(game&&)      = delete;
	~game() noexcept;
	bool init(game_init_args const& args);
	void run();
	[[nodiscard]] context const& ctx() const;
	game& operator=(game const&) = delete;
	game& operator=(game&&)      = delete;

private:
	void shutdown_();

	window window_;
	renderer renderer_;
	SoLoud::Soloud soloud_;
	scene_scheduler scene_scheduler_;
	resources<Texture> textures_;
	util::ziparc aether_resources_;
	// lua::manager lua_manager_;
	// banimation_repository animations_;
	// audio_repository audios_;
	context ctx_;
	bool is_initialized_;
};

} // namespace aether