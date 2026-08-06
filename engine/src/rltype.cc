#include <aether/rltype.hh>
#include <raylib.h>

namespace aether::util {

rlvec2 to_rlvec2(vec2<float> vec2) {
	return {
	        .x = vec2.x,
	        .y = vec2.y,
	};
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
	return {
	        .r = (unsigned char)color.r,
	        .g = (unsigned char)color.g,
	        .b = (unsigned char)color.b,
	        .a = (unsigned char)color.a,
	};
}

rlrect to_rlrect(rect<float> const& rectangle) {
	return {
	        .x      = rectangle.x,
	        .y      = rectangle.y,
	        .width  = rectangle.width,
	        .height = rectangle.height,
	};
}

} // namespace aether::util