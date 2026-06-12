#include <context.hh>
#include <debug/log.hh>
#include <nodes/tilemap.hh>
#include <renderer.hh>
#include <texture2d.hh>
#include <texture_repository.hh>
#include <util/math.hh>

namespace aether {

tilemap::tilemap(context const& ctx, descriptor const& desc)
        : node(ctx)
        , file_arg_(std::string(desc.file))
        , tile_bounds_arg_(static_cast<size<uint32_t>>(util::max(size<int>(1), desc.tile_bounds)))
        , has_antialiasing_(desc.has_antialiasing) {}

tilemap::~tilemap() = default;

void tilemap::toggle_antialiasing(bool val) const {
	SetTextureFilter(texture_->get(), val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
}

vec2<uint32_t> tilemap::tile_count() const {
	auto texture_bounds = texture_->bounds();
	return vec2<uint32_t>(texture_bounds.width / tile_bounds_arg_.width, texture_bounds.height / tile_bounds_arg_.height);
}

void tilemap::seek_tile(vec2<int> tile_index) {
	if (!texture_) {
		AETHER_DEBUGLOG("Attempted to seek tile with nullptr texture");
		return;
	}

	vec2<int> const tile_counti = static_cast<vec2<int>>(tile_count());
	tile_index_                 = static_cast<vec2<uint32_t>>(util::clamp(tile_index, vec2<int>(0), tile_counti - 1));
	texture_source_rect_.x      = tile_bounds_arg_.width * (float)tile_index_.x;
	texture_source_rect_.y      = tile_bounds_arg_.height * (float)tile_index_.y;
}

vec2<uint32_t> tilemap::tile_index() const {
	return tile_index_;
}

bool tilemap::init_() {
	texture_ = ctx_().resource().textures().fetch(file_arg_);

	if (!texture_) {
		AETHER_ERRORLOG("Failed");
		return false;
	}

	auto texture_bounds = texture_->bounds();
	if (tile_bounds_arg_.width > texture_bounds.width || tile_bounds_arg_.height > texture_bounds.height) {
		AETHER_ERRORLOG("Invalid parameters");
		return false;
	}

	texture_source_rect_.width  = (float)tile_bounds_arg_.width;
	texture_source_rect_.height = (float)tile_bounds_arg_.height;
	set_bounds(size<int>((int)tile_bounds_arg_.width, (int)tile_bounds_arg_.height));
	toggle_antialiasing(true);
	schedule_draw();

	return true;
}

void tilemap::draw_(mat3 const& transform, rgba color) {
	ctx_().core().fetch_renderer().draw_texture(texture_->get(), texture_source_rect_, transform, color);
}

} // namespace aether