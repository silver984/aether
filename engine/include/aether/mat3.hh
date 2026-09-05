#pragma once
#include <aether/vec2.hh>

namespace aether {

class mat3 final {
public:
	mat3();
	[[nodiscard]] static mat3 rotation(float radians);
	[[nodiscard]] static mat3 skew(vec2<float> radians);
	[[nodiscard]] static mat3 identity() { return {}; }
	[[nodiscard]] static mat3 translation(vec2<float> t);
	[[nodiscard]] static mat3 scale(vec2<float> s);
	[[nodiscard]] vec2<float> transform_point(vec2<float> p) const;
	[[nodiscard]] vec2<float> translation() const;
	[[nodiscard]] mat3 operator*(mat3 const& o) const;
	[[nodiscard]] float* operator[](size_t i) { return m_[i]; }
	[[nodiscard]] float const* operator[](size_t i) const { return m_[i]; }

private:
	float m_[3][3];
};

} // namespace aether