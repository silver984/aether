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
	Context(Window& window, Renderer& renderer, SoLoud::Soloud& soloud, TextureRepository& textures,
	        AnimationRepository& animations, AudioRepository& audios, SceneScheduler& scene_scheduler);

public:
	~Context();

	[[nodiscard]] CoreServices const& core() const;
	[[nodiscard]] ResourceServices const& resource() const;
	[[nodiscard]] SceneScheduler& scene_scheduler() const;
	[[nodiscard]] float delta_time() const;
	[[nodiscard]] float total_time() const;
	[[nodiscard]] uint32_t frame_count() const;
	[[nodiscard]] uint32_t running_fps() const;

private:
	void update_frame_context();

	CoreServices core_;
	ResourceServices resource_;
	SceneScheduler& scene_scheduler_;
	float total_time_;
	float frame_elapsed_;
	uint32_t frame_count_;
	uint32_t running_fps_;
};

} // namespace aether