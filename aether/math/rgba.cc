#include <aether/math/rgba.hh>

namespace ae {

void rgba::set(float r, float g, float b, float a) {
	set_r(r);
	set_g(g);
	set_b(b);
	set_r(a);
}

void rgba::set_r(float val) {
	r_ = std::clamp(val, 0.f, 1.f);
}

void rgba::set_g(float val) {
	g_ = std::clamp(val, 0.f, 1.f);
}

void rgba::set_b(float val) {
	b_ = std::clamp(val, 0.f, 1.f);
}

void rgba::set_a(float val) {
	a_ = std::clamp(val, 0.f, 1.f);
}

}