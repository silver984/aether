#include <aether/log.hh>
#include <aether/ref.hh>

using namespace aether;

struct fruit : public self_referenceable<fruit> {
	virtual ~fruit() { log<info>({"fruit destroyed"}); }
	void tasty() { log<info>({"{}", fmt::ptr(this->strong_self_().get())}); }
};

struct apple : fruit {
	void crunch() { log<info>({"crunch"}); }
};

struct colored_apple : apple {};
struct red_apple : colored_apple {
	void yummy() { log<info>({"yummy"}); }
};

template <typename T>
void print_count(strong_ref<T> const& val) {
	log<info>({"strong: {} | weak: {}", val.strong_count(), val.weak_count()});
}

int main() {
	{
		strong_ref<fruit> a = ref::strong<red_apple>();
		a->tasty();

		strong_ref<red_apple> b = ref::dynamic_strong_cast<red_apple>(a);
		if (b) {
			b->yummy();
		}

		{
			strong_ref<apple> c = b;
			c->crunch();
			print_count(a);
		}

		print_count(a);
	}

	{
		unique_ref<fruit> d = ref::unique<red_apple>();
		fruit* ptr          = d.get();
		red_apple* casted   = dynamic_cast<red_apple*>(ptr);
		if (casted) {
			casted->crunch();
			casted->yummy();
		}
	}

	return 0;
}