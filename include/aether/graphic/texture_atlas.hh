#ifndef __AETHER_GRAPHIC_TEXTURE_ATLAS_HH__
#define __AETHER_GRAPHIC_TEXTURE_ATLAS_HH__

#include <aether/common/string_map.hh>
#include <aether/math/rect.hh>
#include <aether/math/vec2.hh>
#include <vector>
#include <memory>

struct Texture;

namespace ae {

struct texture_atlas_subtexture final {
	constexpr explicit texture_atlas_subtexture(int i) :
		reference_index(i)
	{}

	int reference_index;
	rect<int> source_rect;
	vec2<int> transform_offset;
};

struct texture_atlas final {
	std::shared_ptr<Texture> texture;
	string_map<std::vector<texture_atlas_subtexture>> subtextures;
};

}

#endif