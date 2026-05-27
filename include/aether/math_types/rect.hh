#pragma once
#include <aether/math_types/math_type.hh>
#include <aether/math_types/numeric_type.hh>
#include <cstddef>

namespace ae {

template <numeric_type T>
struct vec2;

template <numeric_type T>
struct size;

template <numeric_type T>
struct rect final : math_type<rect, T, 4> {
	constexpr rect() = default;
	constexpr rect(T val) : x(val), y(val), width(val), height(val) {}
	constexpr rect(T x_val, T y_val, T width_val, T height_val)
	    : x(x_val), y(y_val), width(width_val), height(height_val) {}

	template <numeric_type U>
	[[nodiscard]] constexpr vec2<U> position() const {
		return {static_cast<U>(x), static_cast<U>(y)};
	}

	template <numeric_type U>
	[[nodiscard]] constexpr size<U> bounds() const {
		return {static_cast<U>(width), static_cast<U>(height)};
	};

	constexpr T& operator[](std::size_t i) {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return width;
		default: return height;
		}
	}

	constexpr T const& operator[](std::size_t i) const {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return width;
		default: return height;
		}
	}

	T x      = T{0};
	T y      = T{0};
	T width  = T{0};
	T height = T{0};
};

} // namespace ae