#pragma once
#include <core_services.hh>
#include <cstdint>
#include <memory>
#include <resource_services.hh>

namespace aether {

class game;
class scene_scheduler;

class context final {
	friend class game;

private:
	context(window& _window, renderer& _renderer, SoLoud::Soloud& soloud, texture_repository& textures,
	        animation_repository& animations, audio_repository& audios, scene_scheduler& _scene_scheduler);

public:
	~context();

	[[nodiscard]] core_services const& core() const;
	[[nodiscard]] resource_services const& resource() const;
	[[nodiscard]] scene_scheduler& fetch_scene_scheduler() const;
	[[nodiscard]] float delta_time() const;
	[[nodiscard]] float total_time() const;
	[[nodiscard]] uint32_t frame_count() const;
	[[nodiscard]] uint32_t running_fps() const;

private:
	void update_frame_context();

	core_services core_;
	resource_services resource_;
	scene_scheduler& scene_scheduler_;
	float total_time_;
	float frame_elapsed_;
	uint32_t frame_count_;
	uint32_t running_fps_;
};

} // namespace aether