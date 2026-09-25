#include <aether/log.hh>
#include <aether/ref.hh>

using namespace aether;

int main() {
	strong_ref<int> x = strong_ref<int>::create(32);
	log<info>({"{}", *x});
	return 0;
}