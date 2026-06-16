#include <context.hh>
#include <data/texture2d.hh>
#include <debug/log.hh>
#include <nodes/tilemap.hh>
#include <renderer.hh>
#include <texture_repository.hh>
#include <util/math.hh>

namespace aether {

tilemap::tilemap(context const& ctx, descriptor const& desc)
        : node(ctx)
        , file_arg_(std::string(desc.file))
        , tile_bounds_arg_(util::max(size<int>(1), desc.tile_bounds))
        , has_antialiasing_(desc.has_antialiasing) {}

tilemap::~tilemap() = default;

void tilemap::toggle_antialiasing(bool val) const {
	SetTextureFilter(texture_->get(), val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
}

vec2<int> tilemap::tile_count() const {
	auto const texture_bounds = texture_->bounds();
	return vec2<int>(texture_bounds.width / tile_bounds_arg_.width, texture_bounds.height / tile_bounds_arg_.height);
}

void tilemap::seek_tile(vec2<int> tile_index) {
	if (!texture_) {
		AETHER_DEBUGLOG("Attempted to seek tile with nullptr texture");
		return;
	}

	tile_index_            = util::clamp(tile_index, vec2<int>(0), tile_count() - 1);
	texture_source_rect_.x = tile_bounds_arg_.width * (float)tile_index_.x;
	texture_source_rect_.y = tile_bounds_arg_.height * (float)tile_index_.y;
}

vec2<int> tilemap::tile_index() const {
	return tile_index_;
}

bool tilemap::init_() {
	texture_ = ctx_().textures().fetch(file_arg_);

	if (!texture_) {
		AETHER_ERRORLOG("Failed");
		return false;
	}

	auto const texture_bounds = texture_->bounds();
	if (tile_bounds_arg_.width > texture_bounds.width || tile_bounds_arg_.height > texture_bounds.height) {
		AETHER_ERRORLOG("Invalid parameters");
		return false;
	}

	texture_source_rect_.width  = (float)tile_bounds_arg_.width;
	texture_source_rect_.height = (float)tile_bounds_arg_.height;
	set_bounds(size<int>(tile_bounds_arg_.width, tile_bounds_arg_.height));
	toggle_antialiasing(has_antialiasing_);
	schedule_draw();

	return true;
}

void tilemap::draw_(mat3 const& transform, rgba color) {
	ctx_().get_renderer().draw_texture(texture_->get(), texture_source_rect_, transform, color);
}

} // namespace aether