#include <context.hh>
#include <debug/log.hh>
#include <fmt/format.h>
#include <raylib.h>
#include <raymath.h>
#include <renderer.hh>
#include <rlgl.h>
#include <util/math.hh>
#include <util/rltype.hh>
#include <window.hh>

namespace aether {

renderer::renderer()
        : projection_(mat3::identity()) {
#ifdef AETHER_DEBUG
	last_debug_fps_ = 0;
#endif
}

renderer::~renderer() = default;

void renderer::draw_texture(rltexture2d const& texture, rect<float> source_rect, mat3 const& transform, rgba color) const {
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

	size<float> const texture_bounds = size<float>((float)texture.width, (float)texture.height);

	push_matrix_(transform);
	rlSetTexture(texture.id);
	rlBegin(RL_QUADS);
	define_color_vertex_(color);
	rlNormal3f(0.f, 0.f, 1.f);

	// top left
	{
		vec2<float> coord = source_rect.position();
		if (flip_x) {
			coord.x += source_rect.width;
		}
		define_texture_coord_(coord / texture_bounds);
		define_vertex_(vec2<float>(0.f));
	}

	// bottom left
	{
		vec2<float> coord = source_rect.position();
		coord.y += source_rect.height;
		if (flip_x) {
			coord.x += source_rect.width;
		}
		define_texture_coord_(coord / texture_bounds);
		define_vertex_(vec2<float>(0.f, source_rect.height));
	}

	// bottom right
	{
		vec2<float> coord = source_rect.position() + source_rect.bounds();
		if (flip_x) {
			coord.x -= source_rect.width;
		}
		define_texture_coord_(coord / texture_bounds);
		define_vertex_(vec2<float>(source_rect.width, source_rect.height));
	}

	// top right
	{
		vec2<float> coord = source_rect.position();
		if (!flip_x) {
			coord.x += source_rect.width;
		}
		define_texture_coord_(coord / texture_bounds);
		define_vertex_(vec2<float>(source_rect.width, 0.f));
	}

	rlEnd();
	rlSetTexture(0);
	rlPopMatrix();
}

void renderer::setup2d_() {
	rlDisableBackfaceCulling();
	rlDisableDepthTest();
}

void renderer::update_viewport_(size<int> target_window_size) {
	render_size_ = size<int>(GetRenderWidth(), GetRenderHeight());

	if (last_render_size_ == render_size_ && last_target_window_size_ == target_window_size) {
		return;
	}

	projection_              = calculate_projection_(render_size_, target_window_size);
	last_render_size_        = render_size_;
	last_target_window_size_ = target_window_size;
}

void renderer::start_draw_() {
	BeginDrawing();
	BeginScissorMode(0, 0, render_size_.width, render_size_.height);
	ClearBackground(WHITE);
	push_matrix_(projection_);
}

#ifdef AETHER_DEBUG
void renderer::end_draw_(uint32_t debug_fps) {
	rlPopMatrix();
	update_debug_(debug_fps);
	draw_debug_();
	EndScissorMode();
	EndDrawing();
}

void renderer::update_debug_(uint32_t debug_fps) {
	if (last_debug_fps_ == debug_fps) {
		return;
	}

	debug_text_         = fmt::format("FPS: {}", debug_fps);
	Vector2 const m     = MeasureTextEx(GetFontDefault(), debug_text_.c_str(), 10.f, 1.f);
	debug_text_measure_ = vec2<int>((int)std::round(m.x), (int)std::round(m.y));
	last_debug_fps_     = debug_fps;
}

void renderer::draw_debug_() {
	if (debug_text_.empty()) {
		return;
	}

	DrawRectangle(4, 4, debug_text_measure_.x + 9, debug_text_measure_.y + 7, Color{0, 0, 0, 64});
	DrawText(debug_text_.c_str(), 8, 8, 10, WHITE);
}
#else
void renderer::end_draw_() {
	rlPopMatrix();
	EndScissorMode();
	EndDrawing();
}
#endif

void renderer::push_matrix_(mat3 const& matrix) const {
	rlPushMatrix();
	rlmat4 const m = util::to_rlmat4(matrix);
	rlMultMatrixf(MatrixToFloat(m));
}

void renderer::define_color_vertex_(rgba color) const {
	rlrgba const c = util::to_rlrgba(color);
	rlColor4ub(c.r, c.g, c.b, c.a);
}

void renderer::define_vertex_(vec2<float> position) const {
	rlVertex2f(position.x, position.y);
}

void renderer::define_texture_coord_(vec2<float> position) const {
	rlTexCoord2f(position.x, position.y);
}

mat3 renderer::calculate_projection_(size<int> render_size, size<int> target_window_size) const {
	vec2<float> const scale_ratio =
	        vec2<float>(render_size.width / (float)target_window_size.width, render_size.height / (float)target_window_size.height);
	float const scale_factor         = std::min(scale_ratio.x, scale_ratio.y);
	vec2<float> const scaled_size    = vec2<float>(target_window_size.width * scale_factor, target_window_size.height * scale_factor);
	vec2<float> const offset         = vec2<float>(render_size.width - scaled_size.x, render_size.height - scaled_size.y);
	vec2<float> const snapped_offset = util::round(offset / 2.f);
	mat3 out                         = mat3::translation(snapped_offset) * mat3::scale(vec2<float>(scale_factor));
	out.m[0][2]                      = std::round(out.m[0][2]);
	out.m[1][2]                      = std::round(out.m[1][2]);
	return out;
}

} // namespace aether