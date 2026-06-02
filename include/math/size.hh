#pragma once
#include <cstddef>
#include <math/numeric.hh>
#include <math/operators.hh>

namespace aether {

template <numeric T>
struct vec2;

template <numeric T>
struct size final : operators<size, T, 2> {
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
		return i == 0 ? width : height;
	}

	constexpr T const& operator[](std::size_t i) const {
		return i == 0 ? width : height;
	}

	T width  = T{0};
	T height = T{0};
};

} // namespace aether