#ifndef __AETHER_SYSTEMS_RENDERER_HH__
#define __AETHER_SYSTEMS_RENDERER_HH__

#include <aether/common/Context.hh>
#include <aether/math/vec2.hh>
#include <aether/math/size.hh>
#include <aether/math/mat3.hh>
#include <aether/math/rgb.hh>
#include <aether/graphics/Texture.hh>
#include <utility>

namespace ae {

class Funkin;

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
    [[nodiscard]] float scale_factor() const;
    [[nodiscard]] vec2<float> scale_ratio() const;
    void set_background_rgba(rgb color, float alpha);
    [[nodiscard]] std::pair<rgb, float> background_rgba() const;
    void draw_texture(Texture const& texture, mat3 const& matrix, rgb color, float alpha) const;

private:
    void update_math(Context const& ctx);
    void start_draw() const;
    void end_draw() const;
#ifdef AETHER_DEBUG
    void draw_debug(Context const& ctx) const;
#endif
    void push_matrix(mat3 const& matrix) const;
    rgb background_color_;
    float background_alpha_;
    float scale_factor_;
    vec2<float> scale_ratio_;
};

}

#endif