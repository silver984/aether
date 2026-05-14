#include <aether/Funkin.hh>
#include <aether/states/TestState.hh>
#include <aether/systems/Director.hh>
#include <aether/version.h>

int main() {
	ae::Funkin aether;

	if (aether.init("FNF: Aether Engine " AETHER_VERSION, ae::size<int>(1280, 720), 240)) {
		auto& ctx = aether.context();

		if (auto director = ctx.director_wref().lock()) {
			director->switch_state(ae::Node::create<TestState>(ctx));
		}

		aether.run();
	}

	return 0;
}