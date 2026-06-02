#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <Context.hh>
#include <objects/TileMap.hh>
#include <raylib.h>
#include <services/Renderer.hh>
#include <services/resource/TextureRepository.hh>
#include <util/math.hh>

namespace aether {

TileMap::TileMap(Context const& ctx, descriptor desc)
    : NodeIdentity<TileMap>(ctx)
    , file_arg_(std::string(desc.file))
    , tile_bounds_arg_(static_cast<size<std::uint32_t>>(util::max({1, 1}, desc.tile_bounds)))
    , has_antialiasing_(desc.has_antialiasing) {}

TileMap::~TileMap() = default;

void TileMap::toggle_antialiasing(bool val) const {
	if (texture_) {
		SetTextureFilter(*texture_, val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
	}
}

vec2<std::uint32_t> TileMap::tile_count() const {
	if (!texture_) {
		return {};
	}

	return {texture_->width / tile_bounds_arg_.width, texture_->height / tile_bounds_arg_.height};
}

void TileMap::seek_tile(vec2<int> tile_index) {
	if (!texture_) {
#ifdef AETHER_VERBOSE_DEBUG
		debuglog("Attempted to seek tile with nullptr texture");
#endif
		return;
	}

	tile_index_ =
	    static_cast<vec2<std::uint32_t>>(util::clamp(tile_index, {}, static_cast<vec2<int>>(tile_count()) - 1));
	texture_source_rect_.x = static_cast<float>(tile_bounds_arg_.width * tile_index_.x);
	texture_source_rect_.y = static_cast<float>(tile_bounds_arg_.height * tile_index_.y);
}

vec2<std::uint32_t> TileMap::tile_index() const {
	return tile_index_;
}

// protected
bool TileMap::init() {
	texture_ = ctx_.texture_repository.fetch(file_arg_);

	if (!texture_) {
#ifdef AETHER_DEBUG
		errorlog("Failed");
#endif
		return false;
	}

	texture_source_rect_.width  = static_cast<float>(tile_bounds_arg_.width);
	texture_source_rect_.height = static_cast<float>(tile_bounds_arg_.height);
	set_bounds({static_cast<int>(tile_bounds_arg_.width), static_cast<int>(tile_bounds_arg_.height)});
	toggle_antialiasing(true);
	enable_draw();

	return true;
}

// protected
void TileMap::draw(mat3 const& transform, rgba color) {
	if (texture_) {
		ctx_.renderer.draw_texture(*texture_, texture_source_rect_, transform, color);
	}
}

} // namespace aether