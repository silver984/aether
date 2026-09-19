#include <aether/log.hh>

int main() {
	aether::log<aether::info>({"hello {}!", "world"});
	aether::log<aether::warn>({"hello {}!", "world"});
	aether::log<aether::error>({"hello {}!", "world"});
	aether::log<aether::debug>({"hello {}!", "world"});
	aether::log<aether::trace>({"hello {}!", "world"});
	return 0;
}
