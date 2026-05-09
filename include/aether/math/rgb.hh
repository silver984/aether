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

    explicit constexpr rgb(uint8_t r, uint8_t g, uint8_t b) :
        r_(static_cast<float>(r) / 255.f),
        g_(static_cast<float>(g) / 255.f),
        b_(static_cast<float>(b) / 255.f)
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

    void set(float r, float g, float b);
    void set(uint8_t r, uint8_t g, uint8_t b);
    void set_r(float val);
    void set_r(uint8_t val);
    void set_g(float val);
    void set_g(uint8_t val);
    void set_b(float val);
    void set_b(uint8_t val);

    [[nodiscard]] constexpr rgb operator*(rgb rhs) const {
        return rgb(
            r_ * rhs.r_,
            g_ * rhs.g_,
            b_ * rhs.b_
        );
    }

private:
    float r_;
    float g_;
    float b_;
};

}

#endif