#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <Context.hh>
#include <objects/TileMap.hh>
#include <raylib.h>
#include <services/core/Renderer.hh>
#include <services/resource/TextureRepository.hh>
#include <util/math.hh>

namespace aether {

TileMap::TileMap(Context const& ctx, descriptor desc)
    : NodeIdentity<TileMap>(ctx)
    , file_arg_(std::string(desc.file))
    , tile_bounds_arg_(static_cast<size<uint32_t>>(util::max(size<int>(1), desc.tile_bounds)))
    , has_antialiasing_(desc.has_antialiasing) {}

TileMap::~TileMap() = default;

void TileMap::toggle_antialiasing(bool val) const {
	if (texture_) {
		SetTextureFilter(*texture_, val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
	}
}

vec2<uint32_t> TileMap::tile_count() const {
	if (!texture_) {
		return {};
	}

	return vec2<uint32_t>(texture_->width / tile_bounds_arg_.width, texture_->height / tile_bounds_arg_.height);
}

void TileMap::seek_tile(vec2<int> tile_index) {
	if (!texture_) {
#ifdef AETHER_VERBOSE_DEBUG
		debuglog("Attempted to seek tile with nullptr texture");
#endif
		return;
	}

	vec2<int> const ltile_count = static_cast<vec2<int>>(tile_count());
	tile_index_                 = static_cast<vec2<uint32_t>>(util::clamp(tile_index, vec2<int>(0.f), ltile_count - 1));
	texture_source_rect_.x      = tile_bounds_arg_.width * (float)tile_index_.x;
	texture_source_rect_.y      = tile_bounds_arg_.height * (float)tile_index_.y;
}

vec2<uint32_t> TileMap::tile_index() const {
	return tile_index_;
}

// protected
bool TileMap::init() {
	texture_ = ctx_.resource_services.texture_repository.fetch(file_arg_);

	if (!texture_) {
#ifdef AETHER_DEBUG
		errorlog("Failed");
#endif
		return false;
	}

	texture_source_rect_.width  = (float)tile_bounds_arg_.width;
	texture_source_rect_.height = (float)tile_bounds_arg_.height;
	set_bounds(size<int>((int)tile_bounds_arg_.width, (int)tile_bounds_arg_.height));
	toggle_antialiasing(true);
	enable_draw();

	return true;
}

// protected
void TileMap::draw(mat3 const& transform, rgba color) {
	if (texture_) {
		ctx_.core_services.renderer.draw_texture(*texture_, texture_source_rect_, transform, color);
	}
}

} // namespace aether