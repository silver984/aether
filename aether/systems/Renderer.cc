#include <aether/systems/Renderer.hh>
#include <raylib.h>
#include <rlgl.h>
#include <fmt/format.h>

namespace {

Matrix raylib_matrix(ae::mat3 const& matrix) {
	return {
		.m0 = matrix.m[0][0], .m4 = matrix.m[0][1], .m8 = 0.f, .m12 = matrix.m[0][2],
		.m1 = matrix.m[1][0], .m5 = matrix.m[1][1], .m9 = 0.f, .m13 = matrix.m[1][2],
		.m2 = 0.f, .m6 = 0.f, .m10 = 1.f, .m14 = 0.f,
		.m3 = 0.f, .m7 = 0.f, .m11 = 0.f, .m15 = 1.f
	};
}

Texture raylib_texture(ae::Texture const& texture) {
	auto texture_bounds = texture.bounds();
	return {
		.id = texture.id(),
		.width = texture_bounds.width,
		.height = texture_bounds.height,
		.mipmaps = texture.mipmaps(),
		.format = static_cast<int>(texture.format())
	};
}

}

namespace ae {

// private
Renderer::Renderer() = default;

// private
Renderer::~Renderer() = default;

size<int> Renderer::bounds() const {
	return {
		.width = GetRenderWidth(),
		.height = GetRenderHeight()
	};
}

void Renderer::draw_texture(Texture const& texture, mat3 const& matrix, float alpha) const {
	push_matrix(matrix);
	
	DrawTexture(
		raylib_texture(texture), 0, 0,
		Color{
			.r = 255,
			.g = 255,
			.b = 255,
			.a = static_cast<uint8_t>(255.f * alpha)
		}
	);

	rlPopMatrix();
}

// private
void Renderer::start_draw() const {
	BeginDrawing();
	// clip bounds
	auto _bounds_ = bounds();
	BeginScissorMode(0, 0, _bounds_.width, _bounds_.height);
	ClearBackground(BLACK);
}

// private
void Renderer::end_draw() const {
	EndScissorMode();
	EndDrawing();
}

#ifdef AETHER_DEBUG
// private
void Renderer::draw_debug(Context const& ctx) const {
	static std::string debug_text;
	debug_text = fmt::format("FPS: {}", ctx.running_fps());
	DrawText(debug_text.c_str(), 5, 5, 10, WHITE);
}
#endif

// private
void Renderer::push_matrix(mat3 const& matrix) const {
	rlPushMatrix();
	Matrix m = raylib_matrix(matrix);
	rlMultMatrixf(&m.m0);
}

}