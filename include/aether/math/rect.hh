#ifndef __AETHER_MATH_RECT_HH__
#define __AETHER_MATH_RECT_HH__

#include <aether/math/math_t.hh>
#include <aether/math/numeric_t.hh>
#include <cstdint>

namespace ae {

template <numeric_t T>
struct vec2;

template <numeric_t T>
struct size;

template <numeric_t T>
struct rect final : math_t<rect, T, 4> {
	constexpr rect() = default;
	constexpr rect(T val) : x(val), y(val), width(val), height(val) {}
	constexpr rect(T x_val, T y_val,
	               T width_val, T height_val) : x(x_val), y(y_val),
	                                            width(width_val), height(height_val) {}

	template <numeric_t U>
	[[nodiscard]] constexpr vec2<U> position() const {
		return {static_cast<U>(x), static_cast<U>(y)};
	}

	template <numeric_t U>
	[[nodiscard]] constexpr size<U> bounds() const {
		return {static_cast<U>(width), static_cast<U>(height)};
	};

	constexpr T& operator[](size_t i) {
		switch (i) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return width;
		default:
			return height;
		}
	}

	constexpr T const& operator[](size_t i) const {
		switch (i) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return width;
		default:
			return height;
		}
	}

	T x = T{0};
	T y = T{0};
	T width = T{0};
	T height = T{0};
};

} // namespace ae

#endif