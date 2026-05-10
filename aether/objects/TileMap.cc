#include <aether/objects/TileMap.hh>

namespace ae {

TileMap::TileMap(Context const& ctx, std::string_view file, size<float> tile_bounds) :
	Sprite(ctx, file),
	tile_bounds_arg_(tile_bounds)
{}

TileMap::~TileMap() = default;

std::string_view ae::TileMap::type() const {
	return "TileMap";
}

}