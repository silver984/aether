#include <aether/common/rl_adapter.hh>
#include <cstdint>

namespace ae::rl {

Texture2D to_Texture2D(Texture const& texture) {
	auto texture_bounds = texture.bounds();
	return Texture2D{
		.id = texture.id(),
		.width = texture_bounds.width,
		.height = texture_bounds.height,
		.mipmaps = texture.mipmaps(),
		.format = static_cast<int>(texture.format())
	};
}

Matrix to_Matrix(mat3 const& matrix) {
	return {
		.m0 = matrix.m[0][0],	.m4 = matrix.m[0][1],	.m8 = 0.f,	.m12 = matrix.m[0][2],
		.m1 = matrix.m[1][0],	.m5 = matrix.m[1][1],	.m9 = 0.f,	.m13 = matrix.m[1][2],
		.m2 = 0.f,				.m6 = 0.f,				.m10 = 1.f, .m14 = 0.f,
		.m3 = 0.f,				.m7 = 0.f,				.m11 = 0.f, .m15 = 1.f
	};
}

Color to_Color(rgb color, float alpha) {
	return Color{
		.r = color.r,
		.g = color.g,
		.b = color.b,
		.a = static_cast<uint8_t>(255.f * alpha),
	};
}

Rectangle to_Rectangle(rect<float> rectangle) {
	return {
		.x = rectangle.x,
		.y = rectangle.y,
		.width = rectangle.width,
		.height = rectangle.height
	};
}

}