#pragma once
#include <aether/resources.hh>

namespace aether {

class game;
class window;
class renderer;
class scene_scheduler;

struct context final {
	window* window;
	renderer* renderer;
	scene_scheduler* scene_scheduler;
	resources<Texture>* textures;
};

} // namespace aether