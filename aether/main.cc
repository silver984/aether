#include <context.hh>
#include <game.hh>
#include <scene_scheduler.hh>
#include <testscene.hh>
#include <version.h>

using namespace aether;

int main() {
	game ae;

	if (ae.init({.window_title = "Aether Engine " AETHER_VERSION, .resolution = size<int>(1280, 720)})) {
		context const& ctx = ae.ctx();
		ctx.get_scene_scheduler().replace_scene(scene::create<testscene>(ctx));
		ae.run();
	}

	return 0;
}