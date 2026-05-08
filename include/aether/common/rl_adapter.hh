#ifndef __AETHER_COMMON_RL_ADAPTER_HH__
#define __AETHER_COMMON_RL_ADAPTER_HH__

#include <aether/math/mat3.hh>
#include <aether/math/rect.hh>
#include <aether/math/rgb.hh>
#include <aether/graphics/Texture.hh>
#include <raylib.h>

namespace ae::rl {

Texture2D to_Texture2D(Texture const& texture);
Matrix to_Matrix(mat3 const& matrix);
Color to_Color(rgb color, float alpha);
Rectangle to_Rectangle(rect<float> rectangle);

}

#endif