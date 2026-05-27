#pragma once
#include <aether/math_types/mat3.hh>
#include <aether/math_types/rect.hh>
#include <aether/math_types/rgba.hh>

struct Vector2;
struct Matrix;
struct Color;
struct Rectangle;

namespace ae::util::rl {

Vector2 as_vector2(vec2<float> vec2);
Matrix as_matrix(mat3 const& matrix);
Color as_color(rgba color);
Rectangle as_rectangle(rect<float> rectangle);

} // namespace ae::util::rl