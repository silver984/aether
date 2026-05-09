#ifndef __AETHER_SYSTEMS_RENDERER_HH__
#define __AETHER_SYSTEMS_RENDERER_HH__

#include <aether/common/Context.hh>
#include <aether/math/vec2.hh>
#include <aether/math/size.hh>
#include <aether/math/mat3.hh>
#include <aether/math/rgb.hh>
#include <cstdint>
#include <utility>

struct Texture;

namespace ae {

class Funkin;
class Window;

class Renderer final {
    friend class Funkin;
public:
    Renderer();
    ~Renderer();
    Renderer(Renderer const&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer const&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    [[nodiscard]] size<int> bounds() const;
    void set_background_rgba(rgb color, float alpha);
    [[nodiscard]] std::pair<rgb, float> background_rgba() const;
    void draw_texture(Texture const& texture, mat3 const& transform, rgb color, float alpha) const;

private:
    void start_draw(Context const& ctx);
    void end_draw(Context const& ctx) const;
#ifdef AETHER_DEBUG
    void draw_debug(Context const& ctx) const;
#endif
    void push_matrix(mat3 const& matrix) const;
    mat3 calculate_transform(std::shared_ptr<Window> window) const;

    rgb background_color_;
    float background_alpha_;
    mat3 transform_;
};

}

#endif