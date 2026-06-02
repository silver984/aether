#pragma once
#include <cstddef>
#include <math/numeric.hh>
#include <math/operators.hh>

namespace aether {

template <numeric T>
struct vec2;

template <numeric T>
struct size final : operators<size, T, 2> {
	using value_type = T;
	constexpr size() = default;
	constexpr size(T v)
	    : width(v)
	    , height(v) {}
	constexpr size(T width_val, T height_val)
	    : width(width_val)
	    , height(height_val) {}

	template <numeric U>
	[[nodiscard]] constexpr operator vec2<U>() const {
		return {static_cast<U>(width), static_cast<U>(height)};
	}

	constexpr T& operator[](std::size_t i) {
		switch (i) {
		case 0: return width;
		case 1: return height;
		default: throw;
		}
	}

	constexpr T const& operator[](std::size_t i) const {
		switch (i) {
		case 0: return width;
		case 1: return height;
		default: throw;
		}
	}

	T width  = T{0};
	T height = T{0};
};

} // namespace aether