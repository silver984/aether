#include <aether/objects/TileMap.hh>
#include <raylib.h>

namespace ae {

TileMap::TileMap(Context const& ctx, std::string_view file, size<float> tile_bounds) :
	Node(ctx),
	file_arg_(std::string(file)),
	tile_bounds_arg_(tile_bounds)
{}

TileMap::~TileMap() = default;

std::string_view ae::TileMap::type() const {
	return "TileMap";
}

void TileMap::toggle_antialiasing(bool val) const {
	if (texture_) {
		SetTextureFilter(*texture_, val ? TextureFilter::TEXTURE_FILTER_BILINEAR : TextureFilter::TEXTURE_FILTER_POINT);
	}
}

}