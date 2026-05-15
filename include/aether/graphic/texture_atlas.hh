#ifndef __AETHER_GRAPHIC_TEXTURE_ATLAS_HH__
#define __AETHER_GRAPHIC_TEXTURE_ATLAS_HH__

#include <aether/common/string_map.hh>
#include <aether/math/rect.hh>
#include <aether/math/vec2.hh>
#include <memory>
#include <vector>

struct Texture;

namespace ae {

struct texture_atlas final {
	struct subtexture final {
		constexpr explicit subtexture(int i) : reference_index(i) {}

		int reference_index;
		rect<int> source_rect;
		vec2<int> offsets;
	};

	std::shared_ptr<Texture> texture;
	string_map<std::vector<subtexture>> subtextures;
};

} // namespace ae

#endif