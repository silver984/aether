#include <Aether.hh>
#include <services/SceneScheduler.hh>
#include <states/TestState.hh>
#include <version.h>

using namespace aether;

int main() {
	Aether aether;

	if (aether.init({.window_title = "Aether Engine " AETHER_VERSION, .resolution = {1280, 720}})) {
		Context const& ctx = aether.context();
		ctx.director.switch_state(Node::create<TestState>(ctx));
		aether.run();
	}

	return 0;
}