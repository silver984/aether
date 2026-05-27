#include <aether/Aether.hh>
#include <aether/debug/log.hh>
#include <aether/states/TestState.hh>
#include <aether/systems/Director.hh>
#include <aether/version.h>

int main() {
	ae::Aether aether;

	if (aether.init("Aether Engine " AETHER_VERSION " for Friday Night Funkin'", {1280, 720}, 240)) {
		auto const& ctx = aether.context();
		ctx.director.switch_state(ae::Node::create<TestState>(ctx));
		aether.run();
	}

	return 0;
}