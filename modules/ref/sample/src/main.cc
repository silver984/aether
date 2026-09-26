#include <aether/log.hh>
#include <aether/ref.hh>

using namespace aether;

struct fruit : public self_ref<fruit> {
	void tasty() { log<info>({"{}", fmt::ptr(this->strong_self_().get())}); }
};

struct apple : fruit {};
struct colored_apple : apple {};
struct red_apple : colored_apple {};

int main() {
	strong_ref<red_apple> a = strong_ref<red_apple>::create();
	a->tasty();
	return 0;
}