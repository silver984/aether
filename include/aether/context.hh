#pragma once
#include <aether/resources.hh>
#include <aether/scene_scheduler.hh>

namespace aether {

struct context final {
	aether::scene_scheduler* scene_scheduler;
	aether::resources<Texture>* textures;
};

} // namespace aether