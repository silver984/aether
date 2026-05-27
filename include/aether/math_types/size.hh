#pragma once
#include <aether/math_types/math_type.hh>
#include <aether/math_types/numeric_type.hh>
#include <cstddef>

namespace ae {

template <numeric_type T>
struct vec2;

template <numeric_type T>
struct size final : math_type<size, T, 2> {
	constexpr size() = default;
	constexpr size(T v) : width(v), height(v) {}
	constexpr size(T width_val, T height_val) : width(width_val), height(height_val) {}

	template <numeric_type U>
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

} // namespace ae