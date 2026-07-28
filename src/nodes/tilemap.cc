#include <context.hh>
#include <data/texture2d.hh>
#include <debug/log.hh>
#include <nodes/tilemap.hh>
#include <renderer.hh>
#include <texture_cache.hh>
#include <util/math.hh>

namespace aether {

tilemap::tilemap(context const& ctx, descriptor const& desc)
        : node(ctx)
        , desc_(desc) {
}

tilemap::~tilemap() = default;

void tilemap::toggle_antialiasing(bool val) const {
	SetTextureFilter(texture_->get(), val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
}

vec2<int> tilemap::tile_count() const {
	auto const texture_bounds = texture_->bounds();
	return vec2<int>(texture_bounds.width / desc_.tile_bounds.width, texture_bounds.height / desc_.tile_bounds.height);
}

void tilemap::seek_tile(vec2<int> tile_index) {
	if (!texture_) {
		AETHER_DEBUGLOG("Attempted to seek tile with nullptr texture");
		return;
	}

	tile_index_            = util::clamp(tile_index, vec2<int>(0), tile_count() - 1);
	texture_source_rect_.x = desc_.tile_bounds.width * (float)tile_index_.x;
	texture_source_rect_.y = desc_.tile_bounds.height * (float)tile_index_.y;
}

vec2<int> tilemap::tile_index() const {
	return tile_index_;
}

bool tilemap::init_() {
	if (!node::init_()) {
		return false;
	}

	texture_ = ctx_().textures().fetch(desc_.file);

	if (!texture_) {
		AETHER_ERRORLOG("Failed");
		return false;
	}

	auto const texture_bounds = texture_->bounds();
	if (desc_.tile_bounds.width > texture_bounds.width || desc_.tile_bounds.height > texture_bounds.height) {
		AETHER_ERRORLOG("Invalid parameters");
		return false;
	}

	texture_source_rect_.width  = (float)desc_.tile_bounds.width;
	texture_source_rect_.height = (float)desc_.tile_bounds.height;
	set_bounds(size<int>(desc_.tile_bounds.width, desc_.tile_bounds.height));
	toggle_antialiasing(desc_.has_antialiasing);
	schedule_draw();

	return true;
}

void tilemap::draw_(mat3 const& transform, rgba color) {
	node::draw_(transform, color);
	ctx_().get_renderer().draw_texture(texture_->get(), texture_source_rect_, transform, color);
}

} // namespace aether