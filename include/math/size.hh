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

	[[nodiscard]] constexpr operator vec2<T>() const {
		return vec2<T>(width, height);
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