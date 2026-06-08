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
using rltexture = Texture;

namespace aether {

class game;
class window;
class context;

class renderer final {
	friend class game;

    private:
	renderer();

    public:
	~renderer();
	renderer(renderer const&)            = delete;
	renderer(renderer&&)                 = delete;
	renderer& operator=(renderer const&) = delete;
	renderer& operator=(renderer&&)      = delete;

	void set_background_rgba(rgba color);
	[[nodiscard]] rgba background_rgba() const;
	void draw_texture(rltexture const& texture, rect<float> source_rect, mat3 const& transform, rgba color) const;

    private:
	void setup(window& window);
	void start_draw();
#ifdef AETHER_DEBUG
	void end_draw(uint32_t running_fps) const;
#else
	void end_draw() const;
#endif
	[[nodiscard]] size<uint32_t> render_bounds() const;
	void push_matrix(mat3 const& matrix) const;
	void define_color_vertex(rgba color) const;
	void define_vertex(vec2<float> position) const;
	void define_texture_coord(vec2<float> position) const;
	[[nodiscard]] mat3 calculate_transform(size<uint32_t> default_window_size) const;

	std::shared_ptr<std::function<void(window&)>> window_resize_callback_;
	mat3 transform_;
	rgba background_rgba_;
};

} // namespace aether