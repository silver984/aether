#include <aether/common/rl_converter.hh>
#include <cmath>
#include <raylib.h>

namespace ae::rl {

Vector2 as_vector2(vec2<float> vec2) {
	return {vec2.x, vec2.y};
}

Matrix as_matrix(mat3 const& matrix) {
	return {.m0 = matrix.m[0][0],
	        .m4 = matrix.m[0][1],
	        .m8 = 0.f,
	        .m12 = matrix.m[0][2],
	        .m1 = matrix.m[1][0],
	        .m5 = matrix.m[1][1],
	        .m9 = 0.f,
	        .m13 = matrix.m[1][2],
	        .m2 = 0.f,
	        .m6 = 0.f,
	        .m10 = 1.f,
	        .m14 = 0.f,
	        .m3 = 0.f,
	        .m7 = 0.f,
	        .m11 = 0.f,
	        .m15 = 1.f};
}

Color as_color(rgba color) {
	return {static_cast<unsigned char>(std::round(255.f * color.r())),
	        static_cast<unsigned char>(std::round(255.f * color.g())),
	        static_cast<unsigned char>(std::round(255.f * color.b())),
	        static_cast<unsigned char>(std::round(255.f * color.a()))};
}

Rectangle as_rectangle(rect<float> rectangle) {
	return {rectangle.x, rectangle.y, rectangle.width, rectangle.height};
}

} // namespace ae::rl