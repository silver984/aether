#include <aether/Funkin.hh>
#include <aether/systems/Director.hh>
#include <aether/states/TestState.hh>

int main() {
	ae::Funkin aether;

	if (aether.init("FNF: Aether Engine", ae::size<int>(1280, 720), 240)) {
		if (
			auto& ctx = aether.context();
			auto director = ctx.director().lock()
		) {
			director->switch_state(ae::Node::create<TestState>(ctx));
		}

		aether.run();
	}

	return 0;
}