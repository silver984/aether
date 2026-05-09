#ifndef __AETHER_COMMON_RL_CONVERTER_HH__
#define __AETHER_COMMON_RL_CONVERTER_HH__

#include <aether/math/mat3.hh>
#include <aether/math/rect.hh>
#include <aether/math/rgb.hh>

struct Vector2;
struct Matrix;
struct Color;
struct Rectangle;

namespace ae::rl {

Vector2 as_vector2(vec2<float> vec2);
Matrix as_matrix(mat3 const& matrix);
Color as_color(rgb color, float alpha);
Rectangle as_rectangle(rect<float> rectangle);

}

#endif