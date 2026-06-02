#pragma once
#include <math_types/mat3.hh>
#include <math_types/rect.hh>
#include <math_types/rgba.hh>

struct Vector2;
struct Matrix;
struct Color;
struct Rectangle;

namespace aether::util::rl {

Vector2 as_vector2(vec2<float> vec2);
Matrix as_matrix(mat3 const& matrix);
Color as_color(rgba color);
Rectangle as_rectangle(rect<float> rectangle);

} // namespace aether::util::rl