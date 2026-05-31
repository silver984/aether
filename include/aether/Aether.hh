#pragma once
#include <aether/math_types/size.hh>
#include <aether/systems/Audio.hh>
#include <aether/systems/Context.hh>
#include <aether/systems/Director.hh>
#include <aether/systems/Renderer.hh>
#include <aether/systems/Window.hh>
#include <aether/systems/repos/TextureAtlasRepo.hh>
#include <aether/systems/repos/TextureRepo.hh>
#include <memory>
#include <string_view>

namespace ae {

class Aether final {
public:
	Aether();
	~Aether();
	Aether(Aether const&)            = delete;
	Aether(Aether&&)                 = delete;
	Aether& operator=(Aether const&) = delete;
	Aether& operator=(Aether&&)      = delete;

	bool init(std::string_view game_title, size<int> game_resolution, int game_fps);
	void run();
	[[nodiscard]] Context const& context();

private:
	void shutdown();

	Window window_;
	Audio audio_;
	Renderer renderer_;
	Director director_;
	TextureRepo texture_repo_;
	TextureAtlasRepo texture_atlas_repo_;
	Context ctx_;
	bool is_initialized_;
};

} // namespace ae