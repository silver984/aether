#pragma once
#include <CoreServices.hh>
#include <ResourceServices.hh>
#include <cstdint>
#include <memory>

namespace aether {

class Aether;
class SceneScheduler;

class Context final {
	friend class Aether;

private:
	Context(Window& _window, Renderer& _renderer, SoLoud::Soloud& _soloud, TextureRepository& _texture_repository,
	        AnimationRepository& _animation_repository, SceneScheduler& _scene_scheduler);

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

	CoreServices core_services;
	ResourceServices resource_services;
	SceneScheduler& scene_scheduler;

private:
	void update_frame_context();

	float total_time_;
	float frame_elapsed_;
	std::uint32_t frame_count_;
	std::uint32_t running_fps_;
};

} // namespace aether