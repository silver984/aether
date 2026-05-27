#pragma once
#include <cstdint>
#include <memory>

namespace ae {

class Aether;
class Window;
class Renderer;
class TextureRepo;
class Resource;
class Director;

class Context final {
	friend class Aether;

private:
	Context(Window& window_ref, Renderer& renderer_ref, TextureRepo& texture_repo_ref, Resource& resource_ref,
	        Director& director_ref);

public:
	~Context();
	Context(Context const&)            = delete;
	Context(Context&&)                 = delete;
	Context& operator=(Context const&) = delete;
	Context& operator=(Context&&)      = delete;

	[[nodiscard]] float delta_time() const;
	[[nodiscard]] float total_time() const;
	[[nodiscard]] uint32_t frame_count() const;
	[[nodiscard]] uint32_t running_fps() const;

	Window& window;
	Renderer& renderer;
	TextureRepo& texture_repo;
	Resource& resource;
	Director& director;

private:
	void update_frame_ctx();

	float total_time_;
	float frame_elapsed_;
	uint32_t frame_count_;
	uint32_t running_fps_;
};

} // namespace ae