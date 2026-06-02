#pragma once
#include <cstdint>
#include <functional>
#include <math/mat3.hh>
#include <math/rect.hh>
#include <math/rgba.hh>
#include <math/size.hh>
#include <math/vec2.hh>
#include <memory>

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
	void setup(Window& window);
	void start_draw();
#ifdef AETHER_DEBUG
	void end_draw(std::uint32_t running_fps) const;
#else
	void end_draw() const;
#endif
	[[nodiscard]] size<std::uint32_t> render_bounds() const;
	void push_matrix(mat3 const& matrix) const;
	void define_color_vertex(rgba color) const;
	void define_vertex(vec2<float> position) const;
	void define_texture_coord(vec2<float> position) const;
	[[nodiscard]] mat3 calculate_transform(size<std::uint32_t> default_window_size) const;

	std::shared_ptr<std::function<void(Window&)>> window_resize_callback_;
	mat3 transform_;
	rgba background_rgba_;
};

} // namespace aether