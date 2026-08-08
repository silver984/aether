#include <aether/game.hh>
#include <aether/scene_scheduler.hh>
#include <aether/testscene.hh>

int main() {
	aether::game engine;

	if (!engine.init({
	            .window_title = "Aether Engine v0.0.1",
	            .resolution   = aether::size<int>(1280, 720),
	    })) {
		return -1;
	}

	auto const& ctx = engine.ctx;
	ctx.scene_scheduler.replace_scene(aether::scene::create<testscene>(ctx));
	engine.run();

	return 0;
}