#pragma once
#include <aether/math_types/mat3.hh>
#include <aether/math_types/rect.hh>
#include <aether/math_types/rgba.hh>
#include <aether/math_types/size.hh>
#include <aether/math_types/vec2.hh>
#include <aether/systems/Context.hh>
#include <cstdint>

struct Texture;

namespace ae {

class Aether;
class Window;

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

	[[nodiscard]] size<int> render_bounds() const;
	void set_background_rgba(rgba color);
	[[nodiscard]] rgba background_rgba() const;
	void draw_texture(Texture const& texture, rect<int> source_rect, mat3 const& transform, rgba color) const;
	void draw_rect(size<int> bounds, mat3 const& transform, rgba color) const;

private:
	void bind_context(Context const& ctx);
	void start_draw();
	void end_draw() const;
#ifdef AETHER_DEBUG
	void draw_debug() const;
#endif
	void push_matrix(mat3 const& matrix) const;
	void define_color_vertex(rgba color) const;
	void define_vertex(vec2<float> position) const;
	void define_texture_coord(vec2<float> position) const;
	mat3 calculate_transform(size<int> screen_size) const;

	Context const* ctx_;
	rgba background_rgba_;
	mat3 transform_;
};

} // namespace ae