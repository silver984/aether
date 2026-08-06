#pragma once
#include <aether/numeric.hh>
#include <algorithm>
#include <cmath>
#include <numbers>

namespace aether {

template <numeric T>
struct vec2;

template <numeric T>
struct size;

} // namespace aether

namespace aether {

template <numeric T>
[[nodiscard]] constexpr T lerp(T a, T b, T t) {
	return a + (b - a) * t;
}

template <indexed_numeric T>
[[nodiscard]] constexpr T lerp(T a, T b, typename T::value_type const& t) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = a[i] + (b[i] - a[i]) * t;
	}
	return out;
}

template <indexed_numeric T>
[[nodiscard]] T damp(T a, T b, typename T::value_type const& l, typename T::value_type const& dt) {
	return lerp(a, b, typename T::value_type{1} - std::exp(-l * dt));
}

template <numeric T>
[[nodiscard]] constexpr int sign(T val) {
	return (val > T{0}) - (val < T{0});
}

template <numeric T>
[[nodiscard]] constexpr T avg(T a, T b) {
	return (a + b) / T{2};
}

template <numeric T>
[[nodiscard]] constexpr T map(T val, T in_min, T in_max, T out_min, T out_max) {
	return out_min + (out_max - out_min) * ((val - in_min) / (in_max - in_min));
}

template <indexed_numeric T>
[[nodiscard]] constexpr T clamp(T val, T min_val, T max_val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::clamp(val[i], min_val[i], max_val[i]);
	}
	return out;
}

template <indexed_numeric T>
[[nodiscard]] T round(T val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::round(val[i]);
	}
	return out;
}

template <indexed_numeric T>
[[nodiscard]] constexpr T ceil(T val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::ceil(val[i]);
	}
	return out;
}

template <indexed_numeric T>
[[nodiscard]] T floor(T val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::floor(val[i]);
	}
	return out;
}

template <indexed_numeric T>
[[nodiscard]] constexpr T abs(T val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::abs(val[i]);
	}
	return out;
}

template <indexed_numeric T>
[[nodiscard]] T sin(T val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::sin(val[i]);
	}
	return out;
}

template <indexed_numeric T>
[[nodiscard]] T cos(T val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::cos(val[i]);
	}
	return out;
}

template <indexed_numeric T>
[[nodiscard]] T tan(T val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::tan(val[i]);
	}
	return out;
}

template <indexed_numeric T>
[[nodiscard]] constexpr T min(T left, T right) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::min(left[i], right[i]);
	}
	return out;
}

template <indexed_numeric T>
[[nodiscard]] constexpr T max(T left, T right) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::max(left[i], right[i]);
	}
	return out;
}

template <indexed_numeric T>
[[nodiscard]] constexpr T reverse(T val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = val[T::capacity() - i - 1];
	}
	return out;
}

template <numeric T>
[[nodiscard]] vec2<T> normalize(vec2<T> val) {
	T const len = std::sqrt(val.x * val.x + val.y * val.y);
	return len == T{0} ? vec2<T>() : vec2<T>(val.x / len, val.y / len);
}

template <numeric T>
[[nodiscard]] T length(vec2<T> val) {
	return std::sqrt(val.x * val.x + val.y * val.y);
}

template <numeric T>
[[nodiscard]] T distance(vec2<T> a, vec2<T> b) {
	return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

template <numeric T>
[[nodiscard]] constexpr T dot(vec2<T> a, vec2<T> b) {
	return a.x * b.x + a.y * b.y;
}

template <numeric T>
[[nodiscard]] constexpr T degrees_to_radians(T deg) {
	return deg * (std::numbers::pi_v<T> / T{180});
}

template <numeric T>
[[nodiscard]] constexpr T radians_to_degrees(T deg) {
	return deg * (T{180} / std::numbers::pi_v<T>);
}

} // namespace aether