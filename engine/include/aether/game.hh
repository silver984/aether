#pragma once
#include <aether/context.hh>
#include <aether/general.h>
#include <aether/ref.hh>
#include <aether/resources.hh>
#include <aether/scene_scheduler.hh>
#include <aether/size.hh>
#include <aether/zip_archive.hh>

#include <soloud.h>

#include <cstdint>
#include <string_view>

namespace aether {

struct game_init_args final {
	std::string_view window_title;
	size<uint32_t> resolution;
	uint32_t fps = 240;
};

class game final {
public:
	game()                       = default;
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

	scene_scheduler scene_scheduler_;
	resources<Texture> textures_;
	SoLoud::Soloud soloud_;
	bool is_initialized_ = false;
};

} // namespace aether