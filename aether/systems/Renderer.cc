#include <aether/common/rl_converter.hh>
#include <aether/systems/Renderer.hh>
#include <aether/systems/Window.hh>
#include <algorithm>
#include <cmath>
#include <fmt/format.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

namespace ae {

// private
Renderer::Renderer(Context const& ctx) : context_(&ctx), background_rgba_(0, 0, 0, 1), transform_(mat3::identity()) {}

// private
Renderer::~Renderer() = default;

size<int> Renderer::bounds() const {
	return {GetRenderWidth(), GetRenderHeight()};
}

void Renderer::set_background_rgba(rgba color) {
	background_rgba_ = color;
}

rgba Renderer::background_rgba() const {
	return background_rgba_;
}

void Renderer::draw_texture(Texture const& texture, rect<int> source_rect, mat3 const& transform, rgba color) const {
	if (texture.id < 1) {
		return;
	}

	bool flip_x = false;

	if (source_rect.width < 0) {
		flip_x            = true;
		source_rect.width = std::abs(source_rect.width);
	}

	if (source_rect.height < 0) {
		source_rect.y -= source_rect.height;
	}

	size<int> texture_bounds = {texture.width, texture.height};

	push_matrix(transform);
	rlSetTexture(texture.id);
	rlBegin(RL_QUADS);
	define_color_vertex(color);
	rlNormal3f(0.f, 0.f, 1.f);

	{ // top left
		vec2<float> coord = source_rect.position<float>();

		if (flip_x) {
			coord.x += static_cast<float>(source_rect.width);
		}

		define_texture_coord(coord / texture_bounds);
		define_vertex({});
	}

	{ // bottom left
		vec2<float> coord = source_rect.position<float>();
		coord.y += static_cast<float>(source_rect.height);

		if (flip_x) {
			coord.x += static_cast<float>(source_rect.width);
		}

		define_texture_coord(coord / texture_bounds);
		define_vertex({0.f, static_cast<float>(source_rect.height)});
	}

	{ // bottom right
		vec2<float> coord = source_rect.position<float>() + source_rect.bounds<float>();

		if (flip_x) {
			coord.x -= static_cast<float>(source_rect.width);
		}

		define_texture_coord(coord / texture_bounds);
		define_vertex({static_cast<float>(source_rect.width), static_cast<float>(source_rect.height)});
	}

	{ // top right
		vec2<float> coord = source_rect.position<float>();

		if (!flip_x) {
			coord.x += static_cast<float>(source_rect.width);
		}

		define_texture_coord(coord / texture_bounds);
		define_vertex({static_cast<float>(source_rect.width), 0.f});
	}

	rlEnd();
	rlSetTexture(0);
	rlPopMatrix();
}

// private
void Renderer::start_draw() {
	BeginDrawing();
	auto lbounds = bounds();
	BeginScissorMode(0, 0, lbounds.width, lbounds.height);
	ClearBackground(rl::as_color(background_rgba_));

	if (auto window = context_->window_wref().lock()) {
		if (window->was_resized()) {
			transform_ = calculate_transform(window);
		}
	}

	push_matrix(transform_);
}

// private
void Renderer::end_draw() const {
	rlPopMatrix();

#ifdef AETHER_DEBUG
	draw_debug();
#endif

	EndScissorMode();
	EndDrawing();
}

#ifdef AETHER_DEBUG
// private
void Renderer::draw_debug() const {
	std::string debug_text = fmt::format("FPS: {}", context_->running_fps());
	DrawText(debug_text.c_str(), 5, 5, 10, WHITE);
}
#endif

// private
void Renderer::push_matrix(mat3 const& matrix) const {
	rlPushMatrix();
	Matrix m = rl::as_matrix(matrix);
	rlMultMatrixf(MatrixToFloat(m));
}

// private
void Renderer::define_color_vertex(rgba color) const {
	Color v = rl::as_color(color);
	rlColor4ub(v.r, v.g, v.b, v.a);
}

// private
void Renderer::define_vertex(vec2<float> position) const {
	rlVertex2f(position.x, position.y);
}

// private
void Renderer::define_texture_coord(vec2<float> position) const {
	rlTexCoord2f(position.x, position.y);
}

// private
mat3 Renderer::calculate_transform(std::shared_ptr<Window> window) const {
	auto screen_size   = window->screen_size();
	auto render_bounds = bounds();

	vec2<float> scale_ratio = {static_cast<float>(render_bounds.width) / screen_size.width,
	                           static_cast<float>(render_bounds.height) / screen_size.height};

	float scale_factor = std::min(scale_ratio.x, scale_ratio.y);

	vec2<float> scaled_size = {screen_size.width * scale_factor, screen_size.height * scale_factor};
	vec2<float> offset      = {render_bounds.width - scaled_size.x, render_bounds.height - scaled_size.y};

	mat3 t = mat3::translation(offset / 2.f);
	mat3 s = mat3::scale(vec2<float>(scale_factor, scale_factor));

	return t * s;
}

} // namespace ae