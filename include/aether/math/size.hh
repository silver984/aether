#pragma once
#include <aether/math/math_t.hh>
#include <aether/math/numeric_t.hh>
#include <cstddef>

namespace ae {

template <numeric_t T>
struct vec2;

template <numeric_t T>
struct size final : math_t<size, T, 2> {
	constexpr size() = default;
	constexpr size(T v) : width(v), height(v) {}
	constexpr size(T width_val, T height_val) : width(width_val), height(height_val) {}

	template <numeric_t U>
	[[nodiscard]] constexpr operator vec2<U>() const {
		return {static_cast<U>(width), static_cast<U>(height)};
	}

	constexpr T& operator[](size_t i) {
		return i == 0 ? width : height;
	}

	constexpr T const& operator[](size_t i) const {
		return i == 0 ? width : height;
	}

	T width  = T{0};
	T height = T{0};
};

} // namespace ae