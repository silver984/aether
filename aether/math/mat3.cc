#include <aether/math/mat3.hh>
#include <cmath>
#include <cstddef>

namespace ae {

mat3::mat3() : m{
    {1.f, 0.f, 0.f},
    {0.f, 1.f, 0.f},
    {0.f, 0.f, 1.f}
} {}

mat3::mat3(
    float m00, float m01, float m02,
    float m10, float m11, float m12,
    float m20, float m21, float m22
) : m{
    {m00, m01, m02},
    {m10, m11, m12},
    {m20, m21, m22}
} {}

mat3 mat3::rotation(float radians) {
    float c = std::cos(radians);
    float s = std::sin(radians);

    return mat3(
        c, -s, 0.f,
        s, c, 0.f,
        0.f, 0.f, 1.f
    );
}

mat3 mat3::skew(vec2<float> radians) {
    vec2<float> tan = {
        .x = std::tan(radians.x),
        .y = std::tan(radians.y)
    };

    return mat3(
        1.f, tan.x, 0.f,
        tan.y, 1.f, 0.f,
        0.f, 0.f, 1.f
    );
}

mat3 mat3::identity() {
    return mat3(
        1.f, 0.f, 0,
        0.f, 1.f, 0,
        0.f, 0.f, 1
    );
}

mat3 mat3::translation(vec2<float> t) {
    return mat3(
        1.f, 0.f, t.x,
        0.f, 1.f, t.y,
        0.f, 0.f, 1.f
    );
}

mat3 mat3::scale(vec2<float> s) {
    return mat3(
        s.x, 0.f, 0,
        0.f, s.y, 0,
        0.f, 0.f, 1.f
    );
}

vec2<float> mat3::translation() const {
    return {
        .x = m[0][2],
        .y = m[1][2]
    };
}

vec2<float> mat3::transform_point(vec2<float> p) const {
    return {
        .x = p.x * m[0][0] + p.y * m[0][1] + m[0][2],
        .y = p.x * m[1][0] + p.y * m[1][1] + m[1][2]
    };
}

mat3 mat3::operator*(const mat3& o) const {
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

}