#pragma once
#include <aether/general.h>
#include <aether/mat3.hh>
#include <aether/rect.hh>
#include <aether/rgba.hh>
#include <aether/service.hh>
#include <aether/size.hh>
#include <aether/vec2.hh>
#include <cstdint>

struct Texture;
using rltexture2d = Texture;

namespace aether {

class game;

class renderer final : public service<renderer> {
	friend class game;

public:
	~renderer() noexcept override;
	void draw_texture(rltexture2d const& texture, rect<float> source_rect, mat3 const& transform, rgba color) const;

private:
	renderer();
	void setup2d_();
	void update_viewport_(size<int> target_window_size);
	void start_draw_();
#if defined(__ae_anydebug__)
	void end_draw_(uint32_t fps, float dt);
#else
	void end_draw_();
#endif
	void push_matrix_(mat3 const& matrix) const;
	void define_color_vertex_(rgba color) const;
	void define_vertex_(vec2<float> position) const;
	void define_texture_coord_(vec2<float> position) const;
	[[nodiscard]] mat3 calculate_projection_(size<int> render_size, size<int> target_size) const;

#if defined(__ae_anydebug__)
	std::string debug_text_;
#endif
	mat3 projection_;
	size<int> render_size_;
	size<int> last_render_size_;
	size<int> last_target_window_size_;
#if defined(__ae_anydebug__)
	vec2<int> debug_text_measure_;
	uint32_t last_fps_;
	float last_dt_;
	float last_heap_usage_;
#endif
};

} // namespace aether