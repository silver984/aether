#pragma once
#include <math_types/rect.hh>
#include <math_types/vec2.hh>

namespace aether {

struct atlas_region final {
	rect<int> source_rect;
	vec2<int> offsets;
	bool is_rotated = false;
};

} // namespace ae