#pragma once
#include <cstdint>
#include <memory>

namespace aether {

class Aether;
class Window;
class Audio;
class Renderer;
class TextureRepository;
class AnimationRepository;
class Director;

class Context final {
	friend class Aether;

private:
	Context(Window& window_ref, Audio& audio_ref, Renderer& renderer_ref, TextureRepository& texture_repo_ref,
	        AnimationRepository& texture_atlas_repo_ref, Director& director_ref);

public:
	~Context();
	Context(Context const&)            = delete;
	Context(Context&&)                 = delete;
	Context& operator=(Context const&) = delete;
	Context& operator=(Context&&)      = delete;

	[[nodiscard]] float delta_time() const;
	[[nodiscard]] float total_time() const;
	[[nodiscard]] std::uint32_t frame_count() const;
	[[nodiscard]] std::uint32_t running_fps() const;

	Window& window;
	Audio& audio;
	Renderer& renderer;
	TextureRepository& texture_repo;
	AnimationRepository& texture_atlas_repo;
	Director& director;

private:
	void update_frame_ctx();

	float total_time_;
	float frame_elapsed_;
	std::uint32_t frame_count_;
	std::uint32_t running_fps_;
};

} // namespace aether