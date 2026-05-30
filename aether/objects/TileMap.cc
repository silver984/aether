#include <aether/debug/log.hh>
#include <aether/objects/TileMap.hh>
#include <aether/systems/Renderer.hh>
#include <aether/systems/repos/TextureRepo.hh>
#include <aether/util/math.hh>
#include <raylib.h>

namespace ae {

TileMap::TileMap(Context const& ctx, std::string_view file, size<int> tile_bounds)
    : NodeIdentity<TileMap>(ctx), file_arg_(std::string(file)),
      tile_bounds_arg_(static_cast<size<std::uint32_t>>(util::math::max({1, 1}, tile_bounds))) {}

TileMap::~TileMap() = default;

void TileMap::toggle_antialiasing(bool val) const {
	if (texture_) {
		using enum TextureFilter;
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
		debuglog("Attempted to seek tile with nullptr texture");
		return;
	}

	tile_index_ =
	    static_cast<vec2<std::uint32_t>>(util::math::clamp(tile_index, {}, static_cast<vec2<int>>(tile_count()) - 1));
	texture_source_rect_.x = tile_bounds_arg_.width * tile_index_.x;
	texture_source_rect_.y = tile_bounds_arg_.height * tile_index_.y;
}

vec2<std::uint32_t> TileMap::tile_index() const {
	return tile_index_;
}

// protected
bool TileMap::init() {
	texture_ = ctx_.texture_repo.fetch(file_arg_);

	if (!texture_) {
		errorlog("Failed");
		return false;
	}

	texture_source_rect_.width  = tile_bounds_arg_.width;
	texture_source_rect_.height = tile_bounds_arg_.height;
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

} // namespace ae