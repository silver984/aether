#pragma once
#include <aether/math/numeric_t.hh>
#include <aether/math/vec2.hh>
#include <aether/math/size.hh>
#include <cmath>
#include <numbers>
#include <algorithm>

namespace ae::math {

template <numeric_t T>
T lerp(T a, T b, T t) {
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
vec2<T> lerp(const vec2<T>& a, const vec2<T>& b, T t) {
    return {
        .x = a.x + (b.x - a.x) * t,
        .y = a.y + (b.y - a.y) * t
    };
}

template <numeric_t T>
vec2<T> clamp(const vec2<T>& val, const vec2<T>& min_val, const vec2<T>& max_val) {
    return {
        .x = std::clamp(val.x, min_val.x, max_val.x),
        .y = std::clamp(val.y, min_val.y, max_val.y)
    };
}

template <numeric_t T>
size<T> clamp(const size<T>& val, const size<T>& min_val, const size<T>& max_val) {
    return {
        .width = std::clamp(val.width, min_val.width, max_val.width),
        .height = std::clamp(val.height, min_val.height, max_val.height)
    };
}

// TODO: min and max

template <numeric_t T>
size<T> max(const size<T>& left, const size<T>& right) {
    return {
        .width = std::max(left.width, right.width),
        .height = std::max(left.height, right.height)
    };
}

template <numeric_t T>
[[nodiscard]] vec2<T> normalize(const vec2<T>& val) {
    T len = std::sqrt(val.x * val.x + val.y * val.y);
    return len == T{0} ? vec2<T>(T{0}, T{0}) : vec2<T>(val.x / len, val.y / len);
}

template <numeric_t T>
[[nodiscard]] T length(const vec2<T>& val) {
    return std::sqrt(val.x * val.x + val.y * val.y);
}

template <numeric_t T>
[[nodiscard]] T distance(const vec2<T>& a, const vec2<T>& b) {
    return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

template <numeric_t T>
[[nodiscard]] T dot(const vec2<T>& a, const vec2<T>& b) {
    return a.x * b.x + a.y * b.y;
}

template <numeric_t T>
vec2<T> damp(const vec2<T>& current, const vec2<T>& target, T lambda, T dt) {
    T t = T{1} - std::exp(-lambda * dt);
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
vec2<T> rotate_point(vec2<T> point, vec2<T> origin, T rotation_degree) {
    T rad = degrees_to_radians(rotation_degree);

    vec2<T> unrotated = vec2<T>(point.x - origin.x, point.y - origin.y);
    vec2<T> rotated = vec2<T>(
        unrotated.x * std::cos(rad) - unrotated.y * std::sin(rad),
        unrotated.x * std::sin(rad) + unrotated.y * std::cos(rad)
    );

    return vec2<T>(rotated.x + origin.x, rotated.y + origin.y);
}

}