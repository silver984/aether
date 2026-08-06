#include <context.hh>
#include <game.hh>
#include <scene_scheduler.hh>
#include <testscene.hh>

int main() {
	aether::game ae;

	if (!ae.init({
	            .window_title = "Aether Engine v0.0.1",
	            .resolution   = aether::size<int>(1280, 720),
	    })) {
		return 0;
	}

	aether::context const& ctx = ae.ctx();
	ctx.scene_scheduler().replace_scene(aether::scene::create<testscene>(ctx));
	ae.run();

	return 0;
}