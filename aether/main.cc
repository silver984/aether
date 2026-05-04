#include <aether/Funkin.hh>
#include <aether/systems/Director.hh>
#include <aether/states/TestState.hh>

int main() {
	ae::Funkin aether;

	if (aether.init("FNF: Aether Engine", ae::size<int>(1280, 720), 240)) {
		auto ctx = aether.context();
		ctx.director()->switch_state(ae::State::create<ae::TestState>(ctx));
		aether.run();
	}

	return 0;
}