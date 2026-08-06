#pragma once
#include <aether/mat3.hh>
#include <aether/rect.hh>
#include <aether/rgba.hh>

struct Vector2;
struct Matrix;
struct Color;
struct Rectangle;
using rlvec2 = Vector2;
using rlmat4 = Matrix;
using rlrgba = Color;
using rlrect = Rectangle;

namespace aether {

rlvec2 to_rlvec2(vec2<float> vec2);
rlmat4 to_rlmat4(mat3 const& matrix);
rlrgba to_rlrgba(rgba color);
rlrect to_rlrect(rect<float> const& rectangle);

} // namespace aether