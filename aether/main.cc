#include <Aether.hh>
#include <Context.hh>
#include <scenes/TestScene.hh>
#include <services/SceneScheduler.hh>
#include <version.h>

using namespace aether;

int main() {
	Aether aether;

	if (aether.init({.window_title = "Aether Engine " AETHER_VERSION, .resolution = {1280, 720}})) {
		Context const& ctx = aether.context();
		ctx.scene_scheduler().replace_scene(Scene::create<TestScene>(ctx));
		aether.run();
	}

	return 0;
}