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

namespace aether::_args {

struct game_init_ final {
	std::string_view window_title;
	size<uint32_t> resolution;
	uint32_t fps = 240;
};

} // namespace aether::_args

namespace aether {

class game final {
public:
	game() = default;

	game(game&&)                 = delete;
	game(game const&)            = delete;
	game& operator=(game&&)      = delete;
	game& operator=(game const&) = delete;

	~game();

	bool init(_args::game_init_ const& args);
	void run(unique_ref<scene> s);
	[[nodiscard]] context ctx();

private:
	scene_scheduler scene_scheduler_;
	resources<Texture> textures_;
	SoLoud::Soloud soloud_;
	bool init_ = false;
};

} // namespace aether