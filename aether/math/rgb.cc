#include <aether/math/rgb.hh>

namespace ae {

void rgb::set(float r, float g, float b) {
	set_r(r);
	set_g(g);
	set_b(b);
}

void rgb::set_r(float val) {
	r_ = std::clamp(val, 0.f, 1.f);
}

void rgb::set_g(float val) {
	g_ = std::clamp(val, 0.f, 1.f);
}

void rgb::set_b(float val) {
	b_ = std::clamp(val, 0.f, 1.f);
}

}