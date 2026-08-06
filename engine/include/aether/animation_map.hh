#pragma once
#include <aether/string.hh>
#include <data/atlas_region.hh>
#include <vector>


namespace aether {

struct animation_data final {
	std::vector<atlas_region> frames;
};

using animation_map = util::string_map<animation_data>;

} // namespace aether