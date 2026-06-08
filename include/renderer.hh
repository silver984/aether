#pragma once
#include <cstdint>
#include <math/mat3.hh>
#include <math/rect.hh>
#include <math/rgba.hh>
#include <math/size.hh>
#include <math/vec2.hh>

struct Texture;
using rltexture = Texture;

namespace aether {

class game;
class window;
class context;

class renderer final {
	friend class game;

public:
	~renderer();
	renderer(renderer const&)            = delete;
	renderer(renderer&&)                 = delete;
	renderer& operator=(renderer const&) = delete;
	renderer& operator=(renderer&&)      = delete;
	void draw_texture(rltexture const& texture, rect<float> source_rect, mat3 const& transform, rgba color) const;

private:
	renderer();
	void setup();
	void update_viewport(size<int> target_window_size);
	void start_draw();
	void end_draw();
	void push_matrix(mat3 const& matrix) const;
	void define_color_vertex(rgba color) const;
	void define_vertex(vec2<float> position) const;
	void define_texture_coord(vec2<float> position) const;
	[[nodiscard]] mat3 calculate_projection(size<int> render_size, size<int> target_window_size) const;

	mat3 projection_;
	size<int> render_size_;
	size<int> last_render_size_;
	size<int> last_target_window_size_;
};

} // namespace aether