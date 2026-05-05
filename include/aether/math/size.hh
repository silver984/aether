#pragma once
#include <aether/math/numeric_t.hh>

namespace ae {

template<numeric_t T>
struct size final {
    T width = T{0};
    T height = T{0};

    [[nodiscard]] constexpr size<T> operator+(size<T> const& rhs) const {
        return {
            .width = width + rhs.width,
            .height = height + rhs.height
        };
    }

    [[nodiscard]] constexpr size<T> operator-(size<T> const& rhs) const {
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

    [[nodiscard]] constexpr size<T> operator*(size<T> const& rhs) const {
        return {
            .width = width * rhs.width,
            .height = height * rhs.height
        };
    }

    [[nodiscard]] constexpr size<T> operator/(size<T> const& rhs) const {
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

    constexpr size<T>& operator+=(size<T> const& rhs) {
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

    constexpr size<T>& operator-=(size<T> const& rhs) {
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

    constexpr size<T>& operator*=(size<T> const& rhs) {
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

    constexpr size<T>& operator/=(size<T> const& rhs) {
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

    constexpr bool operator==(size<T> const& rhs) const {
        return width == rhs.width && height == rhs.height;
    }

    constexpr bool operator!=(size<T> const& rhs) const {
        return !(*this == rhs);
    }

    constexpr bool operator<=(size<T> const& rhs) const {
        return width <= rhs.width && height <= rhs.height;
    }

    constexpr bool operator>=(size<T> const& rhs) const {
        return width >= rhs.width && height >= rhs.height;
    }

    constexpr bool operator<(size<T> const& rhs) const {
        return width < rhs.width && height < rhs.height;
    }

    constexpr bool operator>(size<T> const& rhs) const {
        return width > rhs.width && height > rhs.height;
    }
};

}