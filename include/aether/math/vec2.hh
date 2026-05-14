#ifndef __AETHER_MATH_VEC2_HH__
#define __AETHER_MATH_VEC2_HH__

#include <aether/math/math_t.hh>
#include <aether/math/numeric_t.hh>
#include <cstddef>

namespace ae {

template <numeric_t T>
struct size;

template <numeric_t T>
struct vec2 final : math_t<vec2, T, 2> {
	constexpr vec2() = default;
	constexpr vec2(T v) : x(v), y(v) {}
	constexpr vec2(T x_val, T y_val) : x(x_val), y(y_val) {}

	template <numeric_t U>
	[[nodiscard]] constexpr operator size<U>() const {
		return {static_cast<U>(x), static_cast<U>(y)};
	}

	constexpr T& operator[](size_t i) {
		return i == 0 ? x : y;
	}

	constexpr T const& operator[](size_t i) const {
		return i == 0 ? x : y;
	}

	T x = T{0};
	T y = T{0};
};

} // namespace ae

#endif