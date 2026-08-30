#pragma once
#include <aether/numeric.hh>
#include <aether/operators.hh>
#include <cstddef>
#include <stdexcept>

namespace aether {

template <numeric T>
struct size;

template <numeric T>
struct vec2 final : operators<vec2, T, 2> {
	using value_type = T;

	constexpr vec2()
	        : x(T{0})
	        , y(T{0}) {}
	constexpr vec2(T v)
	        : x(v)
	        , y(v) {}
	constexpr vec2(T xv, T yv)
	        : x(xv)
	        , y(yv) {}

	[[nodiscard]] constexpr operator size<T>() const { return size<T>(x, y); }

	constexpr T& operator[](size_t i) {
		switch (i) {
		case 0:
			return x;
		case 1:
			return y;
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
		default:
			throw std::out_of_range("index out of range");
		}
	}

	T x;
	T y;
};

} // namespace aether