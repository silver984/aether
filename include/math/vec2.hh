#pragma once
#include <cstddef>
#include <math/numeric.hh>
#include <math/operators.hh>

namespace aether {

template <numeric T>
struct size;

template <numeric T>
struct vec2 final : operators<vec2, T, 2> {
	using value_type = T;
	constexpr vec2() = default;
	constexpr vec2(T v)
	    : x(v)
	    , y(v) {}
	constexpr vec2(T x_val, T y_val)
	    : x(x_val)
	    , y(y_val) {}

	template <numeric U>
	[[nodiscard]] constexpr operator size<U>() const {
		return {static_cast<U>(x), static_cast<U>(y)};
	}

	constexpr T& operator[](std::size_t i) {
		switch (i) {
		case 0: return x;
		case 1: return y;
		default: throw;
		}
	}

	constexpr T const& operator[](std::size_t i) const {
		switch (i) {
		case 0: return x;
		case 1: return y;
		default: throw;
		}
	}

	T x = T{0};
	T y = T{0};
};

} // namespace aether