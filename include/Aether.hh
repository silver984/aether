#pragma once
#include <Context.hh>
#include <math/size.hh>
#include <memory>
#include <services/Audio.hh>
#include <services/Director.hh>
#include <services/Renderer.hh>
#include <services/Window.hh>
#include <services/resource/AnimationRepo.hh>
#include <services/resource/TextureRepo.hh>
#include <string_view>

namespace aether {

class Aether final {
public:
	struct init_descriptor final {
		std::string_view window_title;
		size<int> resolution;
		int fps = 240;
	};

	Aether();
	~Aether();
	Aether(Aether const&)            = delete;
	Aether(Aether&&)                 = delete;
	Aether& operator=(Aether const&) = delete;
	Aether& operator=(Aether&&)      = delete;

	bool init(init_descriptor desc);
	void run();
	[[nodiscard]] Context const& context();

private:
	void run_loop();
	void shutdown();

	Window window_;
	Audio audio_;
	Renderer renderer_;
	Director director_;
	TextureRepo texture_repo_;
	AnimationRepo texture_atlas_repo_;
	Context ctx_;
	bool ran_game_loop_;
	bool is_initialized_;
};

} // namespace ae