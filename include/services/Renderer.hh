#pragma once
#include <cstdint>
#include <math_types/mat3.hh>
#include <math_types/rect.hh>
#include <math_types/rgba.hh>
#include <math_types/size.hh>
#include <math_types/vec2.hh>

struct Texture;

namespace aether {

class Aether;
class Window;
class Context;

class Renderer final {
	friend class Aether;

private:
	Renderer();

public:
	~Renderer();
	Renderer(Renderer const&)            = delete;
	Renderer(Renderer&&)                 = delete;
	Renderer& operator=(Renderer const&) = delete;
	Renderer& operator=(Renderer&&)      = delete;

	void set_background_rgba(rgba color);
	[[nodiscard]] rgba background_rgba() const;
	void draw_texture(Texture const& texture, rect<float> source_rect, mat3 const& transform, rgba color) const;

private:
	void start_draw(Window& window);
#ifdef AETHER_DEBUG
	void end_draw(Context const& ctx) const;
	void draw_debug(std::uint32_t running_fps) const;
#else
	void end_draw() const;
#endif
	[[nodiscard]] size<int> render_bounds() const;
	void reset_render_state() const;
	void push_matrix(mat3 const& matrix) const;
	void define_color_vertex(rgba color) const;
	void define_vertex(vec2<float> position) const;
	void define_texture_coord(vec2<float> position) const;
	[[nodiscard]] mat3 calculate_transform(size<int> screen_size) const;

	mat3 transform_;
	rgba background_rgba_;
};

} // namespace ae