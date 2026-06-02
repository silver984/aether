#pragma once
#include <cstddef>
#include <math_types/math_type.hh>
#include <math_types/numeric_type.hh>

namespace aether {

template <numeric_type T>
struct size;

template <numeric_type T>
struct vec2 final : math_type<vec2, T, 2> {
	constexpr vec2() = default;
	constexpr vec2(T v)
	    : x(v)
	    , y(v) {}
	constexpr vec2(T x_val, T y_val)
	    : x(x_val)
	    , y(y_val) {}

	template <numeric_type U>
	[[nodiscard]] constexpr operator size<U>() const {
		return {static_cast<U>(x), static_cast<U>(y)};
	}

	constexpr T& operator[](std::size_t i) {
		return i == 0 ? x : y;
	}

	constexpr T const& operator[](std::size_t i) const {
		return i == 0 ? x : y;
	}

	T x = T{0};
	T y = T{0};
};

} // namespace ae