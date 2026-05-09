#ifndef __AETHER_MATH_SIZE_HH__
#define __AETHER_MATH_SIZE_HH__

#include <aether/math/numeric_t.hh>

namespace ae {

template<numeric_t T>
struct vec2;

template<numeric_t T>
struct size final {
    T width = T{0};
    T height = T{0};

    [[nodiscard]] constexpr size<T> operator+(size<T> rhs) const {
        return {
            .width = width + rhs.width,
            .height = height + rhs.height
        };
    }

    [[nodiscard]] constexpr size<T> operator-(size<T> rhs) const {
        return {
            .width = width - rhs.width,
            .height = height - rhs.height
        };
    }

    [[nodiscard]] constexpr size<T> operator-() const {
        return {
            .width = -width,
            .height = -height
        };
    }

    [[nodiscard]] constexpr size<T> operator*(size<T> rhs) const {
        return {
            .width = width * rhs.width,
            .height = height * rhs.height
        };
    }

    [[nodiscard]] constexpr size<T> operator/(size<T> rhs) const {
        return {
            .width = width / rhs.width,
            .height = height / rhs.height
        };
    }

    template<numeric_t U>
    [[nodiscard]] constexpr size<T> operator+(U rhs) const {
        return {
            .width = width + rhs,
            .height = height + rhs
        };
    }

    template<numeric_t U>
    [[nodiscard]] constexpr size<T> operator-(U rhs) const {
        return {
            .width = width - rhs,
            .height = height - rhs
        };
    }

    template<numeric_t U>
    [[nodiscard]] constexpr size<T> operator*(U rhs) const {
        return {
            .width = width * rhs,
            .height = height * rhs
        };
    }

    template<numeric_t U>
    [[nodiscard]] constexpr size<T> operator/(U rhs) const {
        return {
            .width = width / rhs,
            .height = height / rhs
        };
    }

    constexpr size<T>& operator+=(size<T> rhs) {
        width += rhs.width;
        height += rhs.height;
        return *this;
    }

    template<numeric_t U>
    constexpr size<T>& operator+=(U rhs) {
        width += rhs;
        height += rhs;
        return *this;
    }

    constexpr size<T>& operator-=(size<T> rhs) {
        width -= rhs.width;
        height -= rhs.height;
        return *this;
    }

    template<numeric_t U>
    constexpr size<T>& operator-=(U rhs) {
        width -= rhs;
        height -= rhs;
        return *this;
    }

    constexpr size<T>& operator*=(size<T> rhs) {
        width *= rhs.width;
        height *= rhs.height;
        return *this;
    }

    template<numeric_t U>
    constexpr size<T>& operator*=(U rhs) {
        width *= rhs;
        height *= rhs;
        return *this;
    }

    constexpr size<T>& operator/=(size<T> rhs) {
        width /= rhs.width;
        height /= rhs.height;
        return *this;
    }

    template<numeric_t U>
    constexpr size<T>& operator/=(U rhs) {
        width /= rhs;
        height /= rhs;
        return *this;
    }

    constexpr size<T>& operator++() {
        ++width;
        ++height;
        return *this;
    }

    constexpr size<T> operator++(int) {
        size<T> temp = *this;
        ++(*this);
        return temp;
    }

    constexpr size<T>& operator--() {
        --width;
        --height;
        return *this;
    }

    constexpr size<T> operator--(int) {
        size<T> temp = *this;
        --(*this);
        return temp;
    }

    template<numeric_t U>
    [[nodiscard]] explicit constexpr operator size<U>() const {
        return size<U>{
            .width = static_cast<U>(width),
            .height = static_cast<U>(height)
        };
    }

    template<numeric_t U>
    [[nodiscard]] constexpr operator vec2<U>() const {
        return vec2<U>{
            .x = static_cast<U>(width),
            .y = static_cast<U>(height)
        };
    }

    [[nodiscard]] constexpr bool operator==(size<T> rhs) const {
        return width == rhs.width && height == rhs.height;
    }

    [[nodiscard]] constexpr bool operator!=(size<T> rhs) const {
        return !(*this == rhs);
    }

    [[nodiscard]] constexpr bool operator<=(size<T> rhs) const {
        return width <= rhs.width && height <= rhs.height;
    }

    [[nodiscard]] constexpr bool operator>=(size<T> rhs) const {
        return width >= rhs.width && height >= rhs.height;
    }

    [[nodiscard]] constexpr bool operator<(size<T> rhs) const {
        return width < rhs.width && height < rhs.height;
    }

    [[nodiscard]] constexpr bool operator>(size<T> rhs) const {
        return width > rhs.width && height > rhs.height;
    }
};

}

#endif