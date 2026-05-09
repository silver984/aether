#ifndef __AETHER_COMMON_RL_ADAPTER_HH__
#define __AETHER_COMMON_RL_ADAPTER_HH__

#include <aether/math/mat3.hh>
#include <aether/math/rect.hh>
#include <aether/math/rgb.hh>

struct Vector2;
struct Matrix;
struct Color;
struct Rectangle;

namespace ae::rl {

Vector2 to_Vector2(vec2<float> vec2);
Matrix to_Matrix(mat3 const& matrix);
Color to_Color(rgb color, float alpha);
Rectangle to_Rectangle(rect<float> rectangle);

}

#endif