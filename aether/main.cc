#include <context.hh>
#include <game.hh>
#include <scene_scheduler.hh>
#include <test_scene.hh>
#include <version.h>

using namespace aether;

int main() {
	game aether_engine;

	if (aether_engine.init({.window_title = "Aether Engine " AETHER_VERSION, .resolution = {1280, 720}})) {
		context const& ctx = aether_engine.fetch_context();
		ctx.fetch_scene_scheduler().replace_scene(scene::create<test_scene>(ctx));
		aether_engine.run();
	}

	return 0;
}