#ifndef __AETHER_SYSTEMS_RENDERER_HH__
#define __AETHER_SYSTEMS_RENDERER_HH__

#include <aether/common/Context.hh>
#include <aether/math/mat3.hh>
#include <aether/math/rect.hh>
#include <aether/math/rgba.hh>
#include <aether/math/size.hh>
#include <aether/math/vec2.hh>
#include <cstdint>

struct Texture;

namespace ae {

class Aether;
class Window;

class Renderer final {
	friend class Aether;

public:
	Renderer(Context const& ctx);
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
	void start_draw();
	void end_draw() const;
#ifdef AETHER_DEBUG
	void draw_debug() const;
#endif
	void push_matrix(mat3 const& matrix) const;
	void define_color_vertex(rgba color) const;
	void define_vertex(vec2<float> position) const;
	void define_texture_coord(vec2<float> position) const;
	mat3 calculate_transform(std::shared_ptr<Window> window) const;

	Context const* const context_;
	rgba background_rgba_;
	mat3 transform_;
};

} // namespace ae

#endif