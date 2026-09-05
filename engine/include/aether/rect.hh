#pragma once
#include <aether/numeric.hh>
#include <aether/operators.hh>

#include <cstddef>
#include <stdexcept>

namespace aether {

template <numeric T>
struct vec2;

template <numeric T>
struct size;

template <numeric T>
struct rect final : operators<rect, T, 4> {
	using value_type = T;

	constexpr rect()
	        : x(T{0})
	        , y(T{0})
	        , width(T{0})
	        , height(T{0}) {}
	constexpr rect(T v)
	        : x(v)
	        , y(v)
	        , width(v)
	        , height(v) {}
	constexpr rect(T xv, T yv, T wv, T hv)
	        : x(xv)
	        , y(yv)
	        , width(wv)
	        , height(hv) {}

	[[nodiscard]] constexpr vec2<T> position() const { return vec2<T>(x, y); }
	[[nodiscard]] constexpr size<T> bounds() const { return vec2<T>(width, height); };

	constexpr T& operator[](size_t i) {
		switch (i) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return width;
		case 3:
			return height;
		default:
			throw std::out_of_range("index out of range");
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
		case 3:
			return height;
		default:
			throw std::out_of_range("index out of range");
		}
	}

	T x;
	T y;
	T width;
	T height;
};

} // namespace aether