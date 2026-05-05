#include <aether/systems/Renderer.hh>
#include <aether/common/rl_adapter.hh>
#include <raylib.h>
#include <rlgl.h>
#include <fmt/format.h>

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

void Renderer::draw_texture(Texture const& texture, mat3 const& matrix, rgb const& color, float alpha) const {
	push_matrix(matrix);
	DrawTexture(rl::to_Texture2D(texture), 0, 0, rl::to_Color(color, alpha));
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
	Matrix m = rl::to_Matrix(matrix);
	rlMultMatrixf(&m.m0);
}

}