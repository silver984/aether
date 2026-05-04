#pragma once
#include <aether/common/Context.hh>
#include <aether/math/size.hh>
#include <aether/math/mat3.hh>
#include <aether/graphics/Texture.hh>

namespace ae {

class Funkin;

class Renderer final {
    friend class Funkin;
private:
    Renderer();
    ~Renderer();

public:
    Renderer(Renderer const&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator =(Renderer const&) = delete;
    Renderer& operator =(Renderer&&) = delete;

    [[nodiscard]] size<int> bounds() const;
    void draw_texture(Texture const& texture, mat3 const& matrix, float alpha) const;

private:
    void start_draw() const;
    void end_draw() const;
#ifdef AETHER_DEBUG
    void draw_debug(Context const& ctx) const;
#endif
    void push_matrix(mat3 const& matrix) const;
};

}