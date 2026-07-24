#pragma once
#include <animation_repository.hh>
#include <audio_repository.hh>
#include <context.hh>
#include <lua/manager.hh>
#include <math/size.hh>
#include <renderer.hh>
#include <scene_scheduler.hh>
#include <soloud.h>
#include <string_view>
#include <texture_cache.hh>
#include <window.hh>

namespace aether {

class game final {
public:
	struct init_descriptor final {
		std::string_view window_title;
		size<int> resolution;
		int fps = 240;
	};

	game();
	~game();
	game(game const&)            = delete;
	game(game&&)                 = delete;
	game& operator=(game const&) = delete;
	game& operator=(game&&)      = delete;
	bool init(init_descriptor const& desc);
	void run();
	[[nodiscard]] context const& ctx() const;

private:
	void shutdown_();

	window window_;
	renderer renderer_;
	SoLoud::Soloud soloud_;
	lua::manager lua_manager_;
	scene_scheduler scene_scheduler_;
	texture_cache textures_;
	animation_repository animations_;
	audio_repository audios_;
	context ctx_;
	bool is_initialized_;
};

} // namespace aether