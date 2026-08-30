#pragma once
#include <aether/vec2.hh>

namespace aether {

struct mat3 final {
	mat3();
	explicit mat3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);

	[[nodiscard]] static mat3 rotation(float radians);
	[[nodiscard]] static mat3 skew(vec2<float> radians);
	[[nodiscard]] static mat3 identity();
	[[nodiscard]] static mat3 translation(vec2<float> t);
	[[nodiscard]] static mat3 scale(vec2<float> s);
	[[nodiscard]] vec2<float> transform_point(vec2<float> p) const;
	[[nodiscard]] vec2<float> translation() const;
	[[nodiscard]] mat3 operator*(mat3 const& o) const;
	mat3& operator*=(mat3 const& o);

	float m[3][3];
};

} // namespace aether