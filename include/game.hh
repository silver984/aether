#pragma once
#include <animation_repository.hh>
#include <audio_repository.hh>
#include <chrono>
#include <context.hh>
#include <math/size.hh>
#include <renderer.hh>
#include <scene_scheduler.hh>
#include <sol/state.hpp>
#include <soloud.h>
#include <string_view>
#include <texture_repository.hh>
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
	sol::state lua_;
	scene_scheduler scene_scheduler_;
	texture_repository textures_;
	animation_repository animations_;
	audio_repository audios_;
	context ctx_;
	bool is_initialized_;
};

} // namespace aether