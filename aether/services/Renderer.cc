#include <Context.hh>
#include <algorithm>
#include <cmath>
#include <fmt/format.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <services/Renderer.hh>
#include <services/Window.hh>
#include <util/as_raylib.hh>

// namespace {

// Texture tex_shapes              = {1, 1, 1, 1, 7};
// aether::size<float> tex_shapes_size = {1.f, 1.f};

// } // namespace

namespace aether {

// private
Renderer::Renderer()
    : background_rgba_(0, 0, 0, 255)
    , transform_(mat3::identity()) {}

Renderer::~Renderer() = default;

void Renderer::set_background_rgba(rgba color) {
	background_rgba_ = color;
}

rgba Renderer::background_rgba() const {
	return background_rgba_;
}

void Renderer::draw_texture(Texture const& texture, rect<float> source_rect, mat3 const& transform, rgba color) const {
	if (texture.id < 1) {
		return;
	}

	bool flip_x = false;

	if (source_rect.width < 0.f) {
		flip_x            = true;
		source_rect.width = std::abs(source_rect.width);
	}

	if (source_rect.height < 0) {
		source_rect.y -= source_rect.height;
	}

	size<int> const texture_bounds = {texture.width, texture.height};

	push_matrix(transform);
	rlSetTexture(texture.id);
	rlBegin(RL_QUADS);
	define_color_vertex(color);
	rlNormal3f(0.f, 0.f, 1.f);

	{ // top left
		vec2<float> coord = source_rect.position<float>();

		if (flip_x) {
			coord.x += source_rect.width;
		}

		define_texture_coord(coord / texture_bounds);
		define_vertex({});
	}

	{ // bottom left
		vec2<float> coord = source_rect.position<float>();
		coord.y += source_rect.height;

		if (flip_x) {
			coord.x += source_rect.width;
		}

		define_texture_coord(coord / texture_bounds);
		define_vertex({0.f, source_rect.height});
	}

	{ // bottom right
		vec2<float> coord = source_rect.position<float>() + source_rect.bounds<float>();

		if (flip_x) {
			coord.x -= source_rect.width;
		}

		define_texture_coord(coord / texture_bounds);
		define_vertex({source_rect.width, source_rect.height});
	}

	{ // top right
		vec2<float> coord = source_rect.position<float>();

		if (!flip_x) {
			coord.x += source_rect.width;
		}

		define_texture_coord(coord / texture_bounds);
		define_vertex({source_rect.width, 0.f});
	}

	rlEnd();
	rlSetTexture(0);
	rlPopMatrix();
}

// private
void Renderer::start_draw(Window& window) {
	rlDisableBackfaceCulling();
	BeginDrawing();
	auto lrender_bounds = render_bounds();
	BeginScissorMode(0, 0, lrender_bounds.width, lrender_bounds.height);
	ClearBackground(util::as_color(background_rgba_));

	if (window.was_resized()) {
		transform_ = calculate_transform(window.screen_size());
	}

	push_matrix(transform_);
}

#ifdef AETHER_DEBUG
// private
void Renderer::end_draw(Context const& ctx) const {
	rlPopMatrix();
	draw_debug(ctx.running_fps());
	EndScissorMode();
	EndDrawing();
}

// private
void Renderer::draw_debug(std::uint32_t running_fps) const {
	std::string const debug_text = fmt::format("FPS: {}", running_fps);
	DrawText(debug_text.c_str(), 5, 5, 10, WHITE);
}
#else
// private
void Renderer::end_draw() const {
	rlPopMatrix();
	EndScissorMode();
	EndDrawing();
}
#endif

// private
size<int> Renderer::render_bounds() const {
	return {GetRenderWidth(), GetRenderHeight()};
}

// private
void Renderer::reset_render_state() const {
	rlDisableBackfaceCulling();
	rlDisableDepthTest();
}

// private
void Renderer::push_matrix(mat3 const& matrix) const {
	rlPushMatrix();
	Matrix m = util::as_matrix(matrix);
	rlMultMatrixf(MatrixToFloat(m));
}

// private
void Renderer::define_color_vertex(rgba color) const {
	Color v = util::as_color(color);
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
mat3 Renderer::calculate_transform(size<int> screen_size) const {
	size<int> const lrender_bounds = render_bounds();

	vec2<float> const scale_ratio = {static_cast<float>(lrender_bounds.width) / screen_size.width,
	                                 static_cast<float>(lrender_bounds.height) / screen_size.height};

	float const scale_factor = std::min(scale_ratio.x, scale_ratio.y);

	vec2<float> const scaled_size    = {screen_size.width * scale_factor, screen_size.height * scale_factor};
	vec2<float> const offset         = {lrender_bounds.width - scaled_size.x, lrender_bounds.height - scaled_size.y};
	vec2<float> const snapped_offset = {std::round(offset.x / 2.f), std::round(offset.y / 2.f)};

	mat3 const t   = mat3::translation(snapped_offset);
	mat3 const s   = mat3::scale({scale_factor, scale_factor});
	mat3 result    = t * s;
	result.m[0][2] = std::round(result.m[0][2]);
	result.m[1][2] = std::round(result.m[1][2]);

	return result;
}

} // namespace ae