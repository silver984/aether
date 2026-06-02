#include <Aether.hh>
#include <services/Director.hh>
#include <states/TestState.hh>
#include <version.h>

using namespace aether;

int main() {
	Aether aether;

	if (aether.init("Aether Engine " AETHER_VERSION " for Friday Night Funkin'", {1280, 720}, 240)) {
		auto const& ctx = aether.context();
		ctx.director.switch_state(Node::create<TestState>(ctx));
		aether.run();
	}

	return 0;
}