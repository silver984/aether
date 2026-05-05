#pragma once
#include <aether/math/mat3.hh>
#include <aether/graphics/rgb.hh>
#include <aether/graphics/Texture.hh>
#include <raylib.h>

namespace ae::rl {

Texture2D to_Texture2D(Texture const& texture);
Matrix to_Matrix(mat3 const& matrix);
Color to_Color(rgb const& color, float alpha);

}