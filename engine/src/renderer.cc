#include <aether/context.hh>
#include <aether/heap.hh>
#include <aether/log.hh>
#include <aether/math.hh>
#include <aether/renderer.hh>
#include <aether/rltype.hh>
#include <aether/window.hh>
#include <fmt/format.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

namespace aether {

renderer::renderer()
        : projection_(mat3::identity()) {
#if defined(AETHER_ENGINE_HAS_DEBUG)
	last_fps_        = 0;
	last_heap_usage_ = 0.f;
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

	size<float> const texture_bounds  = size<float>((float)texture.width, (float)texture.height);
	vec2<float> const source_position = source_rect.position();

	push_matrix_(transform);
	rlSetTexture(texture.id);
	rlBegin(RL_QUADS);
	define_color_vertex_(color);
	rlNormal3f(0.f, 0.f, 1.f);

	// top left
	{
		vec2<float> coord = source_position;
		if (flip_x) {
			coord.x += source_rect.width;
		}
		define_texture_coord_(coord / texture_bounds);
		define_vertex_(vec2<float>(0.f));
	}

	// bottom left
	{
		vec2<float> coord = source_position;
		coord.y += source_rect.height;
		if (flip_x) {
			coord.x += source_rect.width;
		}
		define_texture_coord_(coord / texture_bounds);
		define_vertex_(vec2<float>(0.f, source_rect.height));
	}

	// bottom right
	{
		vec2<float> coord = source_position + source_rect.bounds();
		if (flip_x) {
			coord.x -= source_rect.width;
		}
		define_texture_coord_(coord / texture_bounds);
		define_vertex_(vec2<float>(source_rect.width, source_rect.height));
	}

	// top right
	{
		vec2<float> coord = source_position;
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

void renderer::update_viewport_(size<int> target_size) {
	render_size_ = size<int>(GetRenderWidth(), GetRenderHeight());
	if (last_render_size_ == render_size_ && last_target_window_size_ == target_size) {
		return;
	}
	projection_              = calculate_projection_(render_size_, target_size);
	last_render_size_        = render_size_;
	last_target_window_size_ = target_size;
}

void renderer::start_draw_() {
	BeginDrawing();
	BeginScissorMode(0, 0, render_size_.width, render_size_.height);
	ClearBackground(WHITE);
	push_matrix_(projection_);
}

#if defined(AETHER_ENGINE_HAS_DEBUG)
void renderer::end_draw_(uint32_t fps, float dt) {
	rlPopMatrix();

	float const heap_usage   = heap::usage() / (1024.f * 1024.f);
	bool const is_text_dirty = last_fps_ != fps || last_dt_ != dt || last_heap_usage_ != heap_usage;

	if (is_text_dirty) {
		debug_text_ = fmt::format("FPS: {}\nFRAME: {:.0f}MS\nHEAP: {:.2f}MiB", fps, dt * 1000.f, heap_usage);

		Vector2 const measurement = MeasureTextEx(GetFontDefault(), debug_text_.c_str(), 10.f, 1.f);
		debug_text_measure_       = vec2<int>((int)std::round(measurement.x), (int)std::round(measurement.y));

		last_fps_        = fps;
		last_dt_         = dt;
		last_heap_usage_ = heap_usage;
	}

	if (!debug_text_.empty()) {
		DrawRectangle(4, 4, debug_text_measure_.x + 9, debug_text_measure_.y + 7, Color{0, 0, 0, 64});
		DrawText(debug_text_.c_str(), 8, 8, 10, WHITE);
	}

	EndScissorMode();
	EndDrawing();
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

mat3 renderer::calculate_projection_(size<int> render_size, size<int> target_size) const {
	vec2<float> const scale_ratio =
	        vec2<float>(render_size.width / (float)target_size.width, render_size.height / (float)target_size.height);

	float const scale_factor = std::min(scale_ratio.x, scale_ratio.y);

	vec2<float> const scaled_size    = vec2<float>(target_size.width * scale_factor, target_size.height * scale_factor);
	vec2<float> const offset         = vec2<float>(render_size.width - scaled_size.x, render_size.height - scaled_size.y);
	vec2<float> const snapped_offset = util::round(offset / 2.f);

	mat3 out    = mat3::translation(snapped_offset) * mat3::scale(vec2<float>(scale_factor));
	out.m[0][2] = std::round(out.m[0][2]);
	out.m[1][2] = std::round(out.m[1][2]);

	return out;
}

} // namespace aether