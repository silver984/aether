#ifndef __AETHER_MATH_VEC2_HH__
#define __AETHER_MATH_VEC2_HH__

#include <aether/math/numeric_t.hh>

namespace ae {

template <numeric_t T>
struct size;

template <numeric_t T>
struct vec2 final {
	T x = T{0};
	T y = T{0};

	[[nodiscard]] constexpr vec2<T> operator+(vec2<T> rhs) const {
		return {
		    .x = x + rhs.x,
		    .y = y + rhs.y};
	}

	[[nodiscard]] constexpr vec2<T> operator-(vec2<T> rhs) const {
		return {
		    .x = x - rhs.x,
		    .y = y - rhs.y};
	}

	[[nodiscard]] constexpr vec2<T> operator-() const {
		return {
		    .x = -x,
		    .y = -y};
	}

	[[nodiscard]] constexpr vec2<T> operator*(vec2<T> rhs) const {
		return {
		    .x = x * rhs.x,
		    .y = y * rhs.y};
	}

	[[nodiscard]] constexpr vec2<T> operator/(vec2<T> rhs) const {
		return {
		    .x = x / rhs.x,
		    .y = y / rhs.y};
	}

	template <numeric_t U>
	[[nodiscard]] constexpr vec2<T> operator+(U rhs) const {
		T const s = static_cast<T>(rhs);
		return {
		    .x = x + s,
		    .y = y + s};
	}

	template <numeric_t U>
	[[nodiscard]] constexpr vec2<T> operator-(U rhs) const {
		T const s = static_cast<T>(rhs);
		return {
		    .x = x - s,
		    .y = y - s};
	}

	template <numeric_t U>
	[[nodiscard]] constexpr vec2<T> operator*(U rhs) const {
		T const s = static_cast<T>(rhs);
		return {
		    .x = x * s,
		    .y = y * s};
	}

	template <numeric_t U>
	[[nodiscard]] constexpr vec2<T> operator/(U rhs) const {
		T const s = static_cast<T>(rhs);
		return {
		    .x = x / s,
		    .y = y / s};
	}

	constexpr vec2<T>& operator+=(vec2<T> rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	template <numeric_t U>
	constexpr vec2<T>& operator+=(U rhs) {
		T const s = static_cast<T>(rhs);
		x += s;
		y += s;
		return *this;
	}

	constexpr vec2<T>& operator-=(vec2<T> rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	template <numeric_t U>
	constexpr vec2<T>& operator-=(U rhs) {
		T const s = static_cast<T>(rhs);
		x -= s;
		y -= s;
		return *this;
	}

	constexpr vec2<T>& operator*=(vec2<T> rhs) {
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	template <numeric_t U>
	constexpr vec2<T>& operator*=(U rhs) {
		T const s = static_cast<T>(rhs);
		x *= s;
		y *= s;
		return *this;
	}

	constexpr vec2<T>& operator/=(vec2<T> rhs) {
		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}

	template <numeric_t U>
	constexpr vec2<T>& operator/=(U rhs) {
		T const s = static_cast<T>(rhs);
		x /= s;
		y /= s;
		return *this;
	}

	constexpr vec2<T>& operator++() {
		++x;
		++y;
		return *this;
	}

	constexpr vec2<T> operator++(int) {
		vec2<T> temp = *this;
		++(*this);
		return temp;
	}

	constexpr vec2<T>& operator--() {
		--x;
		--y;
		return *this;
	}

	constexpr vec2<T> operator--(int) {
		vec2<T> temp = *this;
		--(*this);
		return temp;
	}

	template <numeric_t U>
	[[nodiscard]] explicit constexpr operator vec2<U>() const {
		return vec2<U>{
		    .x = static_cast<U>(x),
		    .y = static_cast<U>(y)};
	}

	template <numeric_t U>
	[[nodiscard]] constexpr operator size<U>() const {
		return size<U>{
		    .width = static_cast<U>(x),
		    .height = static_cast<U>(y)};
	}

	[[nodiscard]] constexpr bool operator==(vec2<T> rhs) const {
		return x == rhs.x && y == rhs.y;
	}

	[[nodiscard]] constexpr bool operator!=(vec2<T> rhs) const {
		return !(*this == rhs);
	}

	[[nodiscard]] constexpr bool operator<=(vec2<T> rhs) const {
		return x <= rhs.x && y <= rhs.y;
	}

	[[nodiscard]] constexpr bool operator>=(vec2<T> rhs) const {
		return x >= rhs.x && y >= rhs.y;
	}

	[[nodiscard]] constexpr bool operator<(vec2<T> rhs) const {
		return x < rhs.x && y < rhs.y;
	}

	[[nodiscard]] constexpr bool operator>(vec2<T> rhs) const {
		return x > rhs.x && y > rhs.y;
	}
};

} // namespace ae

#endif