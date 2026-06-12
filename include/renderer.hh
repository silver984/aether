#pragma once
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
	void end_draw_();
	void push_matrix_(mat3 const& matrix) const;
	void define_color_vertex_(rgba color) const;
	void define_vertex_(vec2<float> position) const;
	void define_texture_coord_(vec2<float> position) const;
	[[nodiscard]] mat3 calculate_projection_(size<int> render_size, size<int> target_window_size) const;

	mat3 projection_;
	size<int> render_size_;
	size<int> last_render_size_;
	size<int> last_target_window_size_;
};

} // namespace aether