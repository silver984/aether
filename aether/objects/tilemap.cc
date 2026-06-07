#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <context.hh>
#include <objects/tilemap.hh>
#include <raylib.h>
#include <services/core/renderer.hh>
#include <services/resource/texture_repository.hh>
#include <util/math.hh>

namespace aether {

tilemap::tilemap(context const& ctx, descriptor const& desc)
    : node(ctx)
    , file_arg_(std::string(desc.file))
    , tile_bounds_arg_(static_cast<size<uint32_t>>(util::max(size<int>(1), desc.tile_bounds)))
    , has_antialiasing_(desc.has_antialiasing) {}

tilemap::~tilemap() = default;

void tilemap::toggle_antialiasing(bool val) const {
	if (texture_) {
		SetTextureFilter(*texture_, val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
	}
}

vec2<uint32_t> tilemap::tile_count() const {
	if (!texture_) {
		return {};
	}

	return vec2<uint32_t>(texture_->width / tile_bounds_arg_.width, texture_->height / tile_bounds_arg_.height);
}

void tilemap::seek_tile(vec2<int> tile_index) {
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

vec2<uint32_t> tilemap::tile_index() const {
	return tile_index_;
}

// protected
bool tilemap::init() {
	texture_ = ctx().resource().textures().fetch(file_arg_);

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
	schedule_draw();

	return true;
}

// protected
void tilemap::draw(mat3 const& transform, rgba color) {
	if (texture_) {
		ctx().core().fetch_renderer().draw_texture(*texture_, texture_source_rect_, transform, color);
	}
}

} // namespace aether