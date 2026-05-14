#ifndef __AETHER_MATH_RGB_HH__
#define __AETHER_MATH_RGB_HH__

#include <algorithm>
#include <cstdint>

namespace ae {

// i genuinely dont like this type, its kinda ugly

struct rgba final {
	constexpr rgba() : r_(0.f),
					   g_(0.f),
					   b_(0.f),
					   a_(0.f) {}

	explicit constexpr rgba(float r, float g, float b, float a) : r_(std::clamp(r, 0.f, 1.f)),
																  g_(std::clamp(g, 0.f, 1.f)),
																  b_(std::clamp(b, 0.f, 1.f)),
																  a_(std::clamp(a, 0.f, 1.f)) {}

	[[nodiscard]] constexpr float r() const {
		return r_;
	}

	[[nodiscard]] constexpr float g() const {
		return g_;
	}

	[[nodiscard]] constexpr float b() const {
		return b_;
	}

	[[nodiscard]] constexpr float a() const {
		return a_;
	}

	[[nodiscard]] static constexpr float as_float(uint8_t val) {
		return static_cast<float>(val) / 255.f;
	}

	void set(float r, float g, float b, float a);
	void set_r(float val);
	void set_g(float val);
	void set_b(float val);
	void set_a(float val);

	[[nodiscard]] constexpr rgba operator*(rgba rhs) const {
		return rgba(r_ * rhs.r_, g_ * rhs.g_, b_ * rhs.b_, a_ * rhs.a_);
	}

	[[nodiscard]] constexpr bool operator==(rgba rhs) const {
		return r_ == rhs.r_ && g_ == rhs.g_ && b_ == rhs.b_ && a_ == rhs.a_;
	}

  private:
	float r_;
	float g_;
	float b_;
	float a_;
};

} // namespace ae

#endif