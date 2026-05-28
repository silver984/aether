#pragma once
#include <cstdint>
#include <memory>

namespace ae {

class Aether;
class Window;
class Renderer;
class TextureRepo;
class TextureAtlasRepo;
class Director;

class Context final {
	friend class Aether;

private:
	Context(Window& window_ref, Renderer& renderer_ref, TextureRepo& texture_repo_ref,
	        TextureAtlasRepo& texture_atlas_repo_ref, Director& director_ref);

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
	Renderer& renderer;
	TextureRepo& texture_repo;
	TextureAtlasRepo& texture_atlas_repo;
	Director& director;

private:
	void update_frame_ctx();

	float total_time_;
	float frame_elapsed_;
	std::uint32_t frame_count_;
	std::uint32_t running_fps_;
};

} // namespace ae