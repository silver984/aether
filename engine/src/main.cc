#include <aether/game.hh>
#include <aether/testscene.hh>

using namespace aether;

int main() {
	game engine;
	if (!engine.init({
	            .window_title = "Aether Engine v0.0.1",
	            .resolution   = {1280, 720},
	    })) {
		return -1;
	}
	context ctx = engine.ctx();
	engine.run(scene::create<testscene>(ctx));
	return 0;
}