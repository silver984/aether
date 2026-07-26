#include <context.hh>
#include <game.hh>
#include <scene_scheduler.hh>
#include <testscene.hh>
#include <version.h>

int main() {
	aether::game aether;

	if (!aether.init({
	            .window_title = ("Aether Engine " AETHER_VERSION),
	            .resolution   = aether::size<int>(1280, 720),
	    })) {
		return 0;
	}

	aether::context const& ctx = aether.ctx();
	ctx.scene_scheduler().replace_scene(aether::scene::create<testscene>(ctx));
	aether.run();

	return 0;
}