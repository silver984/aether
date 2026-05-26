#pragma once
#include <aether/math/numeric_t.hh>
#include <aether/math/size.hh>
#include <aether/math/vec2.hh>
#include <algorithm>
#include <cmath>
#include <numbers>

namespace ae::math {

template <numeric_t T>
[[nodiscard]] T lerp(T a, T b, T t) {
	return a + (b - a) * t;
}

template <numeric_t T>
[[nodiscard]] int sign(T val) {
	return (val > T{0}) - (val < T{0});
}

template <numeric_t T>
[[nodiscard]] T avg(T a, T b) {
	return (a + b) / T{2};
}

template <numeric_t T>
[[nodiscard]] T map(T val, T in_min, T in_max, T out_min, T out_max) {
	return out_min + (out_max - out_min) * ((val - in_min) / (in_max - in_min));
}

template <numeric_t T>
[[nodiscard]] vec2<T> lerp(vec2<T> a, vec2<T> b, T t) {
	return {a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t};
}

template <numeric_t T>
[[nodiscard]] vec2<T> clamp(vec2<T> val, vec2<T> min_val, vec2<T> max_val) {
	return {std::clamp(val.x, min_val.x, max_val.x), std::clamp(val.y, min_val.y, max_val.y)};
}

template <numeric_t T>
[[nodiscard]] size<T> clamp(size<T> val, size<T> min_val, size<T> max_val) {
	return {std::clamp(val.width, min_val.width, max_val.width),
	        std::clamp(val.height, min_val.height, max_val.height)};
}

// TODO: min and max

template <numeric_t T>
[[nodiscard]] vec2<T> abs(vec2<T> val) {
	return {std::abs(val.x), std::abs(val.y)};
}

template <numeric_t T>
[[nodiscard]] vec2<T> max(vec2<T> left, vec2<T> right) {
	return {std::max(left.x, right.x), std::max(left.y, right.y)};
}

template <numeric_t T>
[[nodiscard]] size<T> max(size<T> left, size<T> right) {
	return {std::max(left.width, right.width), std::max(left.height, right.height)};
}

template <numeric_t T>
[[nodiscard]] size<T> switch_sides(size<T> val) {
	return {val.height, val.width};
}

template <numeric_t T>
[[nodiscard]] vec2<T> switch_sides(vec2<T> val) {
	return {val.y, val.x};
}

template <numeric_t T>
[[nodiscard]] vec2<T> normalize(vec2<T> val) {
	T const len = std::sqrt(val.x * val.x + val.y * val.y);
	return len == T{0} ? vec2<T>(T{0}) : vec2<T>(val.x / len, val.y / len);
}

template <numeric_t T>
[[nodiscard]] T length(vec2<T> val) {
	return std::sqrt(val.x * val.x + val.y * val.y);
}

template <numeric_t T>
[[nodiscard]] T distance(vec2<T> a, vec2<T> b) {
	return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

template <numeric_t T>
[[nodiscard]] T dot(vec2<T> a, vec2<T> b) {
	return a.x * b.x + a.y * b.y;
}

template <numeric_t T>
[[nodiscard]] vec2<T> damp(vec2<T> current, vec2<T> target, T lambda, T dt) {
	T const t = T{1} - std::exp(-lambda * dt);
	return lerp(current, target, t);
}

template <numeric_t T>
[[nodiscard]] T degrees_to_radians(T deg) {
	return deg * (std::numbers::pi_v<T> / T{180});
}

template <numeric_t T>
[[nodiscard]] T radians_to_degrees(T deg) {
	return deg * (T{180} / std::numbers::pi_v<T>);
}

template <numeric_t T>
[[nodiscard]] vec2<T> rotate_point(vec2<T> point, vec2<T> origin, T rotation_degree) {
	T const rad = degrees_to_radians(rotation_degree);

	vec2<T> const unrotated = {point.x - origin.x, point.y - origin.y};

	vec2<T> const rotated = {unrotated.x * std::cos(rad) - unrotated.y * std::sin(rad),
	                         unrotated.x * std::sin(rad) + unrotated.y * std::cos(rad)};

	return {rotated.x + origin.x, rotated.y + origin.y};
}

} // namespace ae::math