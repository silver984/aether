#include <aether/math/rgb.hh>

namespace ae {

void rgb::set(float r, float g, float b) {
	set_r(r);
	set_g(g);
	set_b(b);
}

void rgb::set(uint8_t r, uint8_t g, uint8_t b) {
	set_r(r);
	set_g(g);
	set_b(b);
}

void rgb::set_r(float val) {
	r_ = std::clamp(val, 0.f, 1.f);
}

void rgb::set_r(uint8_t val) {
	r_ = static_cast<float>(val) / 255.f;
}

void rgb::set_g(float val) {
	g_ = std::clamp(val, 0.f, 1.f);
}

void rgb::set_g(uint8_t val) {
	g_ = static_cast<float>(val) / 255.f;
}

void rgb::set_b(float val) {
	b_ = std::clamp(val, 0.f, 1.f);
}

void rgb::set_b(uint8_t val) {
	b_ = static_cast<float>(val) / 255.f;
}

}