#pragma once
#include <aether/math/mat3.hh>
#include <aether/math/rect.hh>
#include <aether/math/rgba.hh>

struct Vector2;
struct Matrix;
struct Color;
struct Rectangle;

namespace ae::rl {

Vector2 as_vector2(vec2<float> vec2);
Matrix as_matrix(mat3 const& matrix);
Color as_color(rgba color);
Rectangle as_rectangle(rect<float> rectangle);

} // namespace ae::rl