#include <aether/systems/Renderer.hh>
#include <aether/systems/Window.hh>
#include <aether/common/rl_adapter.hh>
#include <aether/common/log.hh>
#include <algorithm>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <fmt/format.h>

namespace ae {

// private
Renderer::Renderer() :
	background_alpha_(1.f),
	scale_factor_(1.f),
	scale_ratio_(1.f, 1.f)
{}

// private
Renderer::~Renderer() = default;

size<int> Renderer::bounds() const {
	return {
		.width = GetRenderWidth(),
		.height = GetRenderHeight()
	};
}

float Renderer::scale_factor() const {
	return scale_factor_;
}

vec2<float> Renderer::scale_ratio() const {
	return scale_ratio_;
}

void Renderer::set_background_rgba(rgb color, float alpha) {
	background_color_ = color;
	background_alpha_ = std::clamp(alpha, 0.f, 1.f);
}

std::pair<rgb, float> Renderer::background_rgba() const {
	return {
		background_color_,
		background_alpha_
	};
}

void Renderer::draw_texture(Texture const& texture, mat3 const& matrix, rgb color, float alpha) const {
	push_matrix(matrix);

	// TODO: uh sources for texture atlases?

	auto texture_bounds = texture.bounds();
	Rectangle source = {
		.x = 0,
		.y = 0,
		.width = (float)texture_bounds.width,
		.height = (float)texture_bounds.height,
	};

	Rectangle dest = {
		.x = 0,
		.y = 0,
		.width = source.width,
		.height = source.height,
	};

	DrawTexturePro(
		rl::to_Texture2D(texture),
		source,
		dest,
		Vector2{.x = 0.f, .y = 0.f},
		0.f,
		rl::to_Color(color, alpha)
	);

	rlPopMatrix();
}

// private
void Renderer::update_math(Context const& ctx) {
	auto window = ctx.window().lock();

	if (!window) {
		return;
	}

	auto screen_size = window->screen_size();
	auto render_bounds = bounds();

	scale_ratio_ = {
		.x = screen_size.width > 0 ? static_cast<float>(render_bounds.width) / screen_size.width : 0.f,
		.y = screen_size.height > 0 ? static_cast<float>(render_bounds.height) / screen_size.height : 0.f
	};

	scale_factor_ = std::min(scale_ratio_.x, scale_ratio_.y);
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
	rlMultMatrixf(MatrixToFloat(m));
}

}