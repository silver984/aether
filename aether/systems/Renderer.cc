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
	transform_(mat3::identity())
{}

// private
Renderer::~Renderer() = default;

size<int> Renderer::bounds() const {
	return {
		.width = GetRenderWidth(),
		.height = GetRenderHeight()
	};
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

void Renderer::draw_texture(Texture const& texture, mat3 const& transform, rgb color, float alpha) const {
	push_matrix(transform);

	// TODO: uh sources for texture atlases?

	Rectangle source = {
		.x = 0,
		.y = 0,
		.width = static_cast<float>(texture.width),
		.height = static_cast<float>(texture.height),
	};

	Rectangle dest = {
		.x = 0,
		.y = 0,
		.width = source.width,
		.height = source.height,
	};

	DrawTexturePro(
		texture,
		source,
		dest,
		Vector2{.x = 0.f, .y = 0.f},
		0.f,
		rl::to_Color(color, alpha)
	);

	rlPopMatrix();
}

// private
void Renderer::start_draw(Context const& ctx) {
	BeginDrawing();
	// clip bounds
	auto _bounds_ = bounds();
	BeginScissorMode(0, 0, _bounds_.width, _bounds_.height);
	ClearBackground(rl::to_Color(background_color_, background_alpha_));

	if (auto window = ctx.window().lock()) {
		if (window->was_resized()) {
			transform_ = calculate_transform(window);
		}
	}

	push_matrix(transform_);
}

// private
void Renderer::end_draw(Context const& ctx) const {
	rlPopMatrix();

#ifdef AETHER_DEBUG
	draw_debug(ctx);
#endif

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

// private
mat3 Renderer::calculate_transform(std::shared_ptr<Window> window) const {
	if (!window) {
		return mat3::identity();
	}

	auto screen_size = window->screen_size();
	auto render_bounds = bounds();

	if (screen_size.width <= 0 || screen_size.height <= 0) {
		return mat3::identity();
	}

	vec2<float> scale_ratio = {
		.x = static_cast<float>(render_bounds.width) / screen_size.width,
		.y = static_cast<float>(render_bounds.height) / screen_size.height
	};

	float scale_factor = std::min(scale_ratio.x, scale_ratio.y);

	vec2<float> scaled_size = {
		screen_size.width * scale_factor,
		screen_size.height * scale_factor
	};

	vec2<float> offset = {
		.x = (render_bounds.width - scaled_size.x) * 0.5f,
		.y = (render_bounds.height - scaled_size.y) * 0.5f
	};

	mat3 scale = mat3::scale(vec2<float>(scale_factor, scale_factor));
	mat3 translate = mat3::translation(offset);

	return translate * scale;
}

}