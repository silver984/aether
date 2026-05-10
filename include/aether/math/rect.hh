#ifndef __AETHER_MATH_RECT_HH__
#define __AETHER_MATH_RECT_HH__

#include <aether/math/numeric_t.hh>

namespace ae {

template<numeric_t T>
struct vec2;

template<numeric_t T>
struct size;

template<numeric_t T>
struct rect {
    T x = T{0};
    T y = T{0};
    T width = T{0};
    T height = T{0};

    template<numeric_t U>
    [[nodiscard]] constexpr vec2<U> position() const {
        return {
            .x = x,
            .y = y
        };
    };

    template<numeric_t U>
    [[nodiscard]] constexpr size<U> bounds() const {
        return {
            .width = width,
            .height = height
        };
    };

    [[nodiscard]] constexpr rect<T> operator+(rect<T> rhs) const {
        return {
            .x = x + rhs.x,
            .y = y + rhs.y,
            .width = width + rhs.width,
            .height = height + rhs.height
        };
    }

    [[nodiscard]] constexpr rect<T> operator-(rect<T> rhs) const {
        return {
            .x = x - rhs.x,
            .y = y - rhs.y,
            .width = width - rhs.width,
            .height = height - rhs.height
        };
    }

    [[nodiscard]] constexpr rect<T> operator-() const {
        return {
            .x = -x,
            .y = -y,
            .width = -width,
            .height = -height
        };
    }

    [[nodiscard]] constexpr rect<T> operator*(rect<T> rhs) const {
        return {
            .x = x * rhs.x,
            .y = y * rhs.y,
            .width = width * rhs.width,
            .height = height * rhs.height
        };
    }

    [[nodiscard]] constexpr rect<T> operator/(rect<T> rhs) const {
        return {
            .x = x / rhs.x,
            .y = y / rhs.y,
            .width = width / rhs.width,
            .height = height / rhs.height
        };
    }

    template<numeric_t U>
    [[nodiscard]] constexpr rect<T> operator+(U rhs) const {
        return {
            .x = x + rhs,
            .y = y + rhs,
            .width = width + rhs,
            .height = height + rhs
        };
    }

    template<numeric_t U>
    [[nodiscard]] constexpr rect<T> operator-(U rhs) const {
        return {
            .x = x - rhs,
            .y = y - rhs,
            .width = width - rhs,
            .height = height - rhs
        };
    }

    template<numeric_t U>
    [[nodiscard]] constexpr rect<T> operator*(U rhs) const {
        return {
            .x = x * rhs,
            .y = y * rhs,
            .width = width * rhs,
            .height = height * rhs
        };
    }

    template<numeric_t U>
    [[nodiscard]] constexpr rect<T> operator/(U rhs) const {
        return {
            .x = x / rhs,
            .y = y / rhs,
            .width = width / rhs,
            .height = height / rhs
        };
    }

    constexpr rect<T>& operator+=(rect<T> rhs) {
        x += rhs.x;
        y += rhs.y;
        width += rhs.width;
        height += rhs.height;
        return *this;
    }

    template<numeric_t U>
    constexpr rect<T>& operator+=(U rhs) {
        x += rhs;
        y += rhs;
        width += rhs;
        height += rhs;
        return *this;
    }

    constexpr rect<T>& operator-=(rect<T> rhs) {
        x -= rhs.x;
        y -= rhs.y;
        width -= rhs.width;
        height -= rhs.height;
        return *this;
    }

    template<numeric_t U>
    constexpr rect<T>& operator-=(U rhs) {
        x -= rhs;
        y -= rhs;
        width -= rhs;
        height -= rhs;
        return *this;
    }

    constexpr rect<T>& operator*=(rect<T> rhs) {
        x *= rhs.x;
        y *= rhs.y;
        width *= rhs.width;
        height *= rhs.height;
        return *this;
    }

    template<numeric_t U>
    constexpr rect<T>& operator*=(U rhs) {
        x *= rhs;
        y *= rhs;
        width *= rhs;
        height *= rhs;
        return *this;
    }

    constexpr rect<T>& operator/=(rect<T> rhs) {
        x /= rhs.x;
        y /= rhs.y;
        width /= rhs.width;
        height /= rhs.height;
        return *this;
    }

    template<numeric_t U>
    constexpr rect<T>& operator/=(U rhs) {
        x /= rhs;
        y /= rhs;
        width /= rhs;
        height /= rhs;
        return *this;
    }

    constexpr rect<T>& operator++() {
        ++x;
        ++y;
        ++width;
        ++height;
        return *this;
    }

    constexpr rect<T> operator++(int) {
        rect<T> temp = *this;
        ++(*this);
        return temp;
    }

    constexpr rect<T>& operator--() {
        --x;
        --y;
        --width;
        --height;
        return *this;
    }

    constexpr rect<T> operator--(int) {
        rect<T> temp = *this;
        --(*this);
        return temp;
    }

    template<numeric_t U>
    [[nodiscard]] explicit constexpr operator rect<U>() const {
        return rect<U>{
            .x = static_cast<U>(x),
            .y = static_cast<U>(y),
            .width = static_cast<U>(width),
            .height = static_cast<U>(height)
        };
    }

    [[nodiscard]] constexpr bool operator==(rect<T> rhs) const {
        return x == rhs.x && y == rhs.y && width == rhs.width && height == rhs.height;
    }

    [[nodiscard]] constexpr bool operator!=(rect<T> rhs) const {
        return !(*this == rhs);
    }

    [[nodiscard]] constexpr bool operator<=(rect<T> rhs) const {
        return x <= rhs.x && y <= rhs.y && width <= rhs.width && height <= rhs.height;
    }

    [[nodiscard]] constexpr bool operator>=(rect<T> rhs) const {
        return x >= rhs.x && y >= rhs.y && width >= rhs.width && height >= rhs.height;
    }

    [[nodiscard]] constexpr bool operator<(rect<T> rhs) const {
        return x < rhs.x && y < rhs.y && width < rhs.width && height < rhs.height;
    }

    [[nodiscard]] constexpr bool operator>(rect<T> rhs) const {
        return x > rhs.x && y > rhs.y && width > rhs.width && height > rhs.height;
    }
};

}

#endif