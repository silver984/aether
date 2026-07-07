#pragma once
#include <data/atlas_region.hh>
#include <util/string.hh>
#include <vector>

namespace aether {

struct animation_data final {
	std::vector<atlas_region> frames;
};

using animation_map = util::stringmap<animation_data>;

} // namespace aether