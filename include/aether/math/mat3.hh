#pragma once
#include <aether/math/vec2.hh>

namespace ae {

struct mat3 final {
    constexpr mat3() : m{
        {1.f, 0.f, 0.f},
        {0.f, 1.f, 0.f},
        {0.f, 0.f, 1.f}
    } {}

    constexpr explicit mat3(
        float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22
    ) : m{
        {m00, m01, m02},
        {m10, m11, m12},
        {m20, m21, m22}
    } {}

    [[nodiscard]] static mat3 rotation(float radians);
    [[nodiscard]] static mat3 skew(vec2<float> const& radians);
    [[nodiscard]] static mat3 identity();
    [[nodiscard]] static mat3 translation(vec2<float> const& t);
    [[nodiscard]] static mat3 scale(vec2<float> const& s);
    [[nodiscard]] vec2<float> transform_point(vec2<float> const& p) const;
    [[nodiscard]] vec2<float> translation() const;

    [[nodiscard]] constexpr mat3 operator*(const mat3& o) const {
        mat3 r;

        for (size_t row = 0; row < 3; ++row) {
            for (size_t col = 0; col < 3; ++col) {
                r.m[row][col] =
                    m[row][0] * o.m[0][col] +
                    m[row][1] * o.m[1][col] +
                    m[row][2] * o.m[2][col];
            }
        }

        return r;
    }

    float m[3][3];
};

}