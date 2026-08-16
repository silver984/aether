#include <aether/game.hh>
#include <aether/scene_scheduler.hh>
#include <aether/testscene.hh>

using namespace aether;

int main() {
	game engine;

	if (!engine.init({
	            .window_title = "Aether Engine v0.0.1",
	            .resolution   = size<int>(1280, 720),
	    })) {
		return -1;
	}

	scene_scheduler::instance()->replace_scene(scene::create<testscene>());
	engine.run();

	return 0;
}