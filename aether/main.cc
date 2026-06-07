#include <app.hh>
#include <context.hh>
#include <scene_scheduler.hh>
#include <test_scene.hh>
#include <version.h>

using namespace aether;

int main() {
	app aether;

	if (aether.init({.window_title = "Aether Engine " AETHER_VERSION, .resolution = {1280, 720}})) {
		context const& ctx = aether.fetch_context();
		ctx.fetch_scene_scheduler().replace_scene(scene::create<test_scene>(ctx));
		aether.run();
	}

	return 0;
}