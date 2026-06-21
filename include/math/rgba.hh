#pragma once
#include <cstdint>

namespace aether {

struct rgba final {
	constexpr rgba()
	        : r(0)
	        , g(0)
	        , b(0)
	        , a(0) {
	}

	constexpr rgba(uint8_t v)
	        : r(v)
	        , g(v)
	        , b(v)
	        , a(v) {
	}

	constexpr rgba(uint8_t rv, uint8_t gv, uint8_t bv, uint8_t av)
	        : r(rv)
	        , g(gv)
	        , b(bv)
	        , a(av) {
	}

	[[nodiscard]] constexpr rgba operator*(rgba rhs) {
		auto mul = [](uint8_t a, uint8_t b) -> uint8_t {
			return (uint8_t)(((uint16_t)a * b) / 255);
		};
		return rgba(mul(r, rhs.r), mul(g, rhs.g), mul(b, rhs.b), mul(a, rhs.a));
	}

	[[nodiscard]] constexpr bool operator==(rgba rhs) const {
		return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
	}

	[[nodiscard]] constexpr bool operator!=(rgba rhs) const {
		return !(*this == rhs);
	}

	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

} // namespace aether