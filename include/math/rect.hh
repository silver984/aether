#pragma once
#include <cstddef>
#include <math/numeric.hh>
#include <math/operators.hh>

namespace aether {

template <numeric T>
struct vec2;

template <numeric T>
struct size;

template <numeric T>
struct rect final : operators<rect, T, 4> {
	using value_type = T;
	constexpr rect() = default;
	constexpr rect(T val)
	    : x(val)
	    , y(val)
	    , width(val)
	    , height(val) {}
	constexpr rect(T x_val, T y_val, T width_val, T height_val)
	    : x(x_val)
	    , y(y_val)
	    , width(width_val)
	    , height(height_val) {}

	[[nodiscard]] constexpr vec2<T> position() const {
		return {x, y};
	}

	[[nodiscard]] constexpr size<T> bounds() const {
		return {width, height};
	};

	constexpr T& operator[](std::size_t i) {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return width;
		case 3: return height;
		default: throw;
		}
	}

	constexpr T const& operator[](std::size_t i) const {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return width;
		case 3: return height;
		default: throw;
		}
	}

	T x      = T{0};
	T y      = T{0};
	T width  = T{0};
	T height = T{0};
};

} // namespace aether