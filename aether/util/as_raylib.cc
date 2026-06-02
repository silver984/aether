#include <cmath>
#include <raylib.h>
#include <util/as_raylib.hh>

namespace aether::util {

Vector2 as_vector2(vec2<float> vec2) {
	return {vec2.x, vec2.y};
}

Matrix as_matrix(mat3 const& matrix) {
	Matrix r{};
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

Color as_color(rgba color) {
	return {color.r, color.g, color.b, color.a};
}

Rectangle as_rectangle(rect<float> rectangle) {
	return {rectangle.x, rectangle.y, rectangle.width, rectangle.height};
}

} // namespace aether::util