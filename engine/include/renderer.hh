#pragma once
#include <config.h>
#include <cstdint>
#include <math/mat3.hh>
#include <math/rect.hh>
#include <math/rgba.hh>
#include <math/size.hh>
#include <math/vec2.hh>

struct Texture;
using rltexture2d = Texture;

namespace aether {

class game;
class window;
class context;

class renderer final {
	friend class game;

public:
	~renderer();
	void draw_texture(rltexture2d const& texture, rect<float> source_rect, mat3 const& transform, rgba color) const;

private:
	renderer();
	void setup2d_();
	void update_viewport_(size<int> target_window_size);
	void start_draw_();
#if defined(AETHER_ENGINE_HAS_DEBUG)
	void end_draw_(uint32_t fps, float dt);
#else
	void end_draw_();
#endif
	void push_matrix_(mat3 const& matrix) const;
	void define_color_vertex_(rgba color) const;
	void define_vertex_(vec2<float> position) const;
	void define_texture_coord_(vec2<float> position) const;
	[[nodiscard]] mat3 calculate_projection_(size<int> render_size, size<int> target_size) const;

#if defined(AETHER_ENGINE_HAS_DEBUG)
	std::string debug_text_;
#endif
	mat3 projection_;
	size<int> render_size_;
	size<int> last_render_size_;
	size<int> last_target_window_size_;
#if defined(AETHER_ENGINE_HAS_DEBUG)
	vec2<int> debug_text_measure_;
	uint32_t last_fps_;
	float last_dt_;
	float last_heap_usage_;
#endif
};

} // namespace aether