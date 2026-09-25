#include <aether/mat3.hh>
#include <aether/math.hh>

#include <cmath>
#include <cstddef>

namespace aether {

mat3::mat3()
        : m_{
                  {1.f, 0.f, 0.f},
                  {0.f, 1.f, 0.f},
                  {0.f, 0.f, 1.f},
          } {}

mat3 mat3::rotation(float radians) {
	float const c = std::cos(radians);
	float const s = std::sin(radians);
	mat3 out;
	out.m_[0][0] = c;
	out.m_[0][1] = -s;
	out.m_[1][0] = s;
	out.m_[1][1] = c;
	return out;
}

mat3 mat3::skew(vec2<float> radians) {
	vec2<float> const t = tan(radians);
	mat3 out;
	out.m_[0][1] = t.x;
	out.m_[1][0] = t.y;
	return out;
}

mat3 mat3::translation(vec2<float> t) {
	mat3 out;
	out.m_[0][2] = t.x;
	out.m_[1][2] = t.y;
	return out;
}

mat3 mat3::scale(vec2<float> s) {
	mat3 out;
	out.m_[0][0] = s.x;
	out.m_[1][1] = s.y;
	return out;
}

vec2<float> mat3::transform_point(vec2<float> p) const {
	return {
	        p.x * m_[0][0] + p.y * m_[0][1] + m_[0][2],
	        p.x * m_[1][0] + p.y * m_[1][1] + m_[1][2],
	};
}

vec2<float> mat3::translation() const {
	return {
	        m_[0][2],
	        m_[1][2],
	};
}

mat3 mat3::operator*(mat3 const& o) const {
	mat3 out;
	for (size_t row = 0; row < 3; ++row) {
		for (size_t col = 0; col < 3; ++col) {
			out.m_[row][col] = m_[row][0] * o.m_[0][col] + m_[row][1] * o.m_[1][col] + m_[row][2] * o.m_[2][col];
		}
	}
	return out;
}

} // namespace aether