#include <context.hh>
#include <fmt/format.h>
#include <raylib.h>
#include <raymath.h>
#include <renderer.hh>
#include <rlgl.h>
#include <util/math.hh>
#include <util/rltype.hh>
#include <window.hh>

namespace aether {

// private
renderer::renderer()
        : window_resize_callback_(std::make_shared<std::function<void(window&)>>())
        , background_rgba_(0, 0, 0, 255)
        , transform_(mat3::identity()) {
	(*window_resize_callback_) = [this](window& window) {
		transform_ = calculate_transform(window.default_size());
	};
}

renderer::~renderer() = default;

void renderer::set_background_rgba(rgba color) {
	background_rgba_ = color;
}

rgba renderer::background_rgba() const {
	return background_rgba_;
}

void renderer::draw_texture(rltexture const& texture, rect<float> source_rect, mat3 const& transform,
                            rgba color) const {
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
		vec2<float> coord = source_rect.position();

		if (flip_x) {
			coord.x += source_rect.width;
		}

		define_texture_coord(coord / texture_bounds);
		define_vertex(vec2<float>(0.f));
	}

	{ // bottom left
		vec2<float> coord = source_rect.position();
		coord.y += source_rect.height;

		if (flip_x) {
			coord.x += source_rect.width;
		}

		define_texture_coord(coord / texture_bounds);
		define_vertex(vec2<float>(0.f, source_rect.height));
	}

	{ // bottom right
		vec2<float> coord = source_rect.position() + source_rect.bounds();

		if (flip_x) {
			coord.x -= source_rect.width;
		}

		define_texture_coord(coord / texture_bounds);
		define_vertex(vec2<float>(source_rect.width, source_rect.height));
	}

	{ // top right
		vec2<float> coord = source_rect.position();

		if (!flip_x) {
			coord.x += source_rect.width;
		}

		define_texture_coord(coord / texture_bounds);
		define_vertex(vec2<float>(source_rect.width, 0.f));
	}

	rlEnd();
	rlSetTexture(0);
	rlPopMatrix();
}

// private
void renderer::setup(window& window) {
	window.on_resize(window_resize_callback_);
	rlDisableBackfaceCulling();
	rlDisableDepthTest();
}

// private
void renderer::start_draw() {
	BeginDrawing();
	size<uint32_t> const lrender_bounds = render_bounds();
	BeginScissorMode(0, 0, (int)lrender_bounds.width, (int)lrender_bounds.height);
	ClearBackground(util::to_rlrgba(background_rgba_));
	push_matrix(transform_);
}

#ifdef AETHER_DEBUG
// private
void renderer::end_draw(uint32_t running_fps) const {
	rlPopMatrix();
	DrawText(fmt::format("FPS: {}", running_fps).c_str(), 5, 5, 10, WHITE);
	EndScissorMode();
	EndDrawing();
}
#else
// private
void renderer::end_draw() const {
	rlPopMatrix();
	EndScissorMode();
	EndDrawing();
}
#endif

// private
size<uint32_t> renderer::render_bounds() const {
	// this should be fine...
	return size<uint32_t>((uint32_t)GetRenderWidth(), (uint32_t)GetRenderHeight());
}

// private
void renderer::push_matrix(mat3 const& matrix) const {
	rlPushMatrix();
	rlmat4 const m = util::to_rlmat4(matrix);
	rlMultMatrixf(MatrixToFloat(m));
}

// private
void renderer::define_color_vertex(rgba color) const {
	rlrgba const v = util::to_rlrgba(color);
	rlColor4ub(v.r, v.g, v.b, v.a);
}

// private
void renderer::define_vertex(vec2<float> position) const {
	rlVertex2f(position.x, position.y);
}

// private
void renderer::define_texture_coord(vec2<float> position) const {
	rlTexCoord2f(position.x, position.y);
}

// private
mat3 renderer::calculate_transform(size<uint32_t> default_window_size) const {
	size<uint32_t> const lrender_bounds = render_bounds();
	vec2<float> const scale_ratio       = vec2<float>(lrender_bounds.width / (float)default_window_size.width,
	                                                  lrender_bounds.height / (float)default_window_size.height);
	float const scale_factor            = std::min(scale_ratio.x, scale_ratio.y);
	vec2<float> const scaled_size =
	        vec2<float>(default_window_size.width * scale_factor, default_window_size.height * scale_factor);
	vec2<float> const offset =
	        vec2<float>(lrender_bounds.width - scaled_size.x, lrender_bounds.height - scaled_size.y);
	vec2<float> const snapped_offset = util::round(offset / 2.f);
	mat3 result                      = mat3::translation(snapped_offset) * mat3::scale(vec2<float>(scale_factor));
	result.m[0][2]                   = std::round(result.m[0][2]);
	result.m[1][2]                   = std::round(result.m[1][2]);
	return result;
}

} // namespace aether