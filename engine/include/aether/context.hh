#pragma once
#include <aether/resources.hh>

namespace aether {

class game;
class window;
class scene_scheduler;

struct context final {
	aether::window* window;
	aether::scene_scheduler* scene_scheduler;
	aether::resources<Texture>* textures;
};

} // namespace aether