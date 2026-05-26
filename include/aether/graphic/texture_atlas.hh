#pragma once
#include <aether/math_types/rect.hh>
#include <aether/math_types/vec2.hh>
#include <aether/util/string_map.hh>
#include <memory>
#include <vector>

struct Texture;

namespace ae {

struct texture_atlas final {
	struct subtexture final {
		constexpr explicit subtexture(int i) : reference_index(i), is_rotated(false) {}

		int reference_index;
		rect<int> source_rect;
		vec2<int> offsets;
		bool is_rotated;
	};

	std::shared_ptr<Texture> texture;
	string_map<std::vector<subtexture>> subtextures;
};

} // namespace ae