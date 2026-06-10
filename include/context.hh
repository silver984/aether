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

private:
	core_services core_;
	resource_services resource_;
	scene_scheduler& scene_scheduler_;
};

} // namespace aether