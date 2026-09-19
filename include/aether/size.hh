#pragma once
#include <aether/numeric.hh>
#include <aether/operators.hh>

#include <cstddef>
#include <stdexcept>

namespace aether {

template <numeric T>
struct vec2;

template <numeric T>
struct size final : operators<size, T, 2> {
	using value_type = T;

	constexpr size()
	        : width(T{0})
	        , height(T{0}) {}
	constexpr size(T v)
	        : width(v)
	        , height(v) {}
	constexpr size(T wv, T hv)
	        : width(wv)
	        , height(hv) {}

	[[nodiscard]] constexpr operator vec2<T>() const { return vec2<T>(width, height); }

	constexpr T& operator[](size_t i) {
		switch (i) {
		case 0:
			return width;
		case 1:
			return height;
		default:
			throw std::out_of_range("index out of range");
		}
	}

	constexpr T const& operator[](size_t i) const {
		switch (i) {
		case 0:
			return width;
		case 1:
			return height;
		default:
			throw std::out_of_range("index out of range");
		}
	}

	T width;
	T height;
};

} // namespace aether