#include <aether/Funkin.hh>

int main() {
	ae::Funkin aether;

	if (aether.init("FNF: Aether Engine", ae::size<int>(1280, 720), 240)) {
		aether.run();
	}

	return 0;
}