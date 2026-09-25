#include <aether/log.hh>

using namespace aether;

int main() {
	log<info>({"hello {}!", "world"});
	log<warn>({"hello {}!", "world"});
	log<error>({"hello {}!", "world"});
	log<debug>({"hello {}!", "world"});
	log<trace>({"hello {}!", "world"});
	return 0;
}
