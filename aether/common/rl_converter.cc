#include <aether/common/rl_converter.hh>
#include <raylib.h>
#include <cstdint>
#include <cmath>

namespace ae::rl {

Vector2 as_vector2(vec2<float> vec2) {
	return {
		.x = vec2.x,
		.y = vec2.y
	};
}

Matrix as_matrix(mat3 const& matrix) {
	return {
		.m0 = matrix.m[0][0],	.m4 = matrix.m[0][1],	.m8 = 0.f,	.m12 = matrix.m[0][2],
		.m1 = matrix.m[1][0],	.m5 = matrix.m[1][1],	.m9 = 0.f,	.m13 = matrix.m[1][2],
		.m2 = 0.f,				.m6 = 0.f,				.m10 = 1.f, .m14 = 0.f,
		.m3 = 0.f,				.m7 = 0.f,				.m11 = 0.f, .m15 = 1.f
	};
}

Color as_color(rgba color) {
	return {
		.r = static_cast<uint8_t>(std::round(255.f * color.r())),
		.g = static_cast<uint8_t>(std::round(255.f * color.g())),
		.b = static_cast<uint8_t>(std::round(255.f * color.b())),
		.a = static_cast<uint8_t>(std::round(255.f * color.a())),
	};
}

Rectangle as_rectangle(rect<float> rectangle) {
	return {
		.x = rectangle.x,
		.y = rectangle.y,
		.width = rectangle.width,
		.height = rectangle.height
	};
}

}