#ifndef __AETHER_MATH_RGB_HH__
#define __AETHER_MATH_RGB_HH__

#include <cstdint>
#include <algorithm>

namespace ae {

struct rgb final {
    constexpr rgb() :
        r_(0.f),
        g_(0.f),
        b_(0.f)
    {}

    explicit constexpr rgb(float r, float g, float b) :
        r_(std::clamp(r, 0.f, 1.f)),
        g_(std::clamp(g, 0.f, 1.f)),
        b_(std::clamp(b, 0.f, 1.f))
    {}

    [[nodiscard]] constexpr float r() const {
        return r_;
    }

    [[nodiscard]] constexpr float g() const {
        return g_;
    }

    [[nodiscard]] constexpr float b() const {
        return b_;
    }

    [[nodiscard]] static constexpr float as_float(uint8_t val) {
        return static_cast<float>(val) / 255.f;
    }

    void set(float r, float g, float b);
    void set_r(float val);
    void set_g(float val);
    void set_b(float val);

    [[nodiscard]] constexpr rgb operator*(rgb rhs) const {
        return rgb(r_ * rhs.r_, g_ * rhs.g_, b_ * rhs.b_);
    }

private:
    float r_;
    float g_;
    float b_;
};

}

#endif