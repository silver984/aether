#pragma once
#include <Context.hh>
#include <math/size.hh>
#include <memory>
#include <services/SceneScheduler.hh>
#include <services/core/AudioManager.hh>
#include <services/core/Renderer.hh>
#include <services/core/Window.hh>
#include <services/resource/AnimationRepository.hh>
#include <services/resource/TextureRepository.hh>
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
	void shutdown();

	Window window_;
	AudioManager audio_manager_;
	Renderer renderer_;
	SceneScheduler scene_scheduler_;
	TextureRepository texture_repository_;
	AnimationRepository animation_repository_;
	Context ctx_;
	bool ran_game_loop_;
	bool is_initialized_;
};

} // namespace aether