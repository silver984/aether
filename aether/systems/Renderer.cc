#include <aether/systems/Renderer.hh>
#include <aether/common/rl_adapter.hh>
#include <algorithm>
#include <raylib.h>
#include <rlgl.h>
#include <fmt/format.h>

namespace ae {

// private
Renderer::Renderer() :
	background_alpha_(1.f)
{}

// private
Renderer::~Renderer() = default;

size<int> Renderer::bounds() const {
	return {
		.width = GetRenderWidth(),
		.height = GetRenderHeight()
	};
}

void Renderer::set_background_rgba(rgb const& color, float alpha) {
	background_color_ = color;
	background_alpha_ = std::clamp(alpha, 0.f, 1.f);
}

std::pair<rgb, float> Renderer::background_rgba() const {
	return {
		background_color_,
		background_alpha_
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
	ClearBackground(rl::to_Color(background_color_, background_alpha_));
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