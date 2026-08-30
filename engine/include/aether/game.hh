#pragma once
#include <aether/context.hh>
#include <aether/general.h>
#include <aether/ref.hh>
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
public:
	game();
	game(game&&)                 = delete;
	game(game const&)            = delete;
	game& operator=(game&&)      = delete;
	game& operator=(game const&) = delete;
	~game();

	bool init(game_init_args const& args);
	void run(unique_ref<scene> s);
	[[nodiscard]] context ctx();

private:
	void shutdown_();

	window window_;
	renderer renderer_;
	scene_scheduler scene_scheduler_;
	resources<Texture> textures_;
	SoLoud::Soloud soloud_;
	bool is_initialized_;
};

} // namespace aether