#pragma once
#include <cstdint>

namespace aether {

struct rgba final {
	constexpr rgba() = default;
	constexpr rgba(uint8_t val)
	    : r(val)
	    , g(val)
	    , b(val)
	    , a(val) {}
	constexpr rgba(uint8_t r_val, uint8_t g_val, uint8_t b_val, uint8_t a_val)
	    : r(r_val)
	    , g(g_val)
	    , b(b_val)
	    , a(a_val) {}

	constexpr rgba operator*(rgba rhs) {
		auto mul = [](uint8_t a, uint8_t b) -> uint8_t {
			return static_cast<uint8_t>((static_cast<uint16_t>(a) * b) / 255);
		};

		return {mul(r, rhs.r), mul(g, rhs.g), mul(b, rhs.b), mul(a, rhs.a)};
	}

	[[nodiscard]] constexpr bool operator==(rgba rhs) const {
		return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
	}

	[[nodiscard]] constexpr bool operator!=(rgba rhs) const {
		return !(*this == rhs);
	}

	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 0;
};

} // namespace aether