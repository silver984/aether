#include <cmath>
#include <raylib.h>
#include <util/rltype.hh>

namespace aether::util {

rlvec2 to_rlvec2(vec2<float> vec2) {
	return {vec2.x, vec2.y};
}

rlmat4 to_rlmat4(mat3 const& matrix) {
	rlmat4 r{};
	r.m0  = matrix.m[0][0];
	r.m4  = matrix.m[0][1];
	r.m12 = matrix.m[0][2];
	r.m1  = matrix.m[1][0];
	r.m5  = matrix.m[1][1];
	r.m13 = matrix.m[1][2];
	r.m10 = 1.f;
	r.m15 = 1.f;
	return r;
}

rlrgba to_rlrgba(rgba color) {
	return {(unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, (unsigned char)color.a};
}

rlrect to_rlrect(rect<float> rectangle) {
	return {rectangle.x, rectangle.y, rectangle.width, rectangle.height};
}

} // namespace aether::util