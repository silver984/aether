#include <aether/objects/TileMap.hh>

namespace ae {

TileMap::TileMap(std::string_view file, size<float> tile_bounds) :
	Sprite(file),
	tile_bounds_arg_(tile_bounds)
{}

TileMap::~TileMap() = default;

std::string_view ae::TileMap::type() const {
	return "TileMap";
}

}