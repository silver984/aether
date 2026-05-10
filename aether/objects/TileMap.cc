#include <aether/objects/TileMap.hh>
#include <aether/systems/Resource.hh>
#include <aether/common/log.hh>
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

// protected
bool TileMap::init() {
	auto resource = context().resource_wref().lock();

	if (!resource) {
		errorlog("Can't reference resource system");
		return false;
	}

	texture_ = resource->load_shared_texture(file_arg_);

	if (!texture_) {
		errorlog("Failed");
		return false;
	}

	return true;
}

}