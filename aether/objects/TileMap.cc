#include <aether/objects/TileMap.hh>
#include <aether/systems/Resource.hh>
#include <aether/systems/Renderer.hh>
#include <aether/common/log.hh>
#include <aether/math/util.hh>
#include <raylib.h>
#include <cmath>

namespace ae {

TileMap::TileMap(Context const& ctx, std::string_view file, size<int> tile_bounds) :
	Node(ctx),
	file_arg_(std::string(file)),
	tile_bounds_arg_(math::max(size<int>(1, 1), tile_bounds))
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

vec2<int> TileMap::tile_count() const {
	if (!texture_) {
		return {
			.x = 0,
			.y = 0
		};
	}

	auto helper = [](int left, int right) -> int {
		return static_cast<int>(std::floor(static_cast<float>(left) / right));
		};

	vec2<int> count = {
		.x = helper(texture_->width, tile_bounds_arg_.width),
		.y = helper(texture_->height, tile_bounds_arg_.height)
	};

	return count;
}

void TileMap::seek_tile(vec2<int> tile_index) {
	tile_index_ = math::max(vec2<int>(0, 0), tile_index);
	texture_source_rect_.x = tile_bounds_arg_.width * tile_index_.x;
	texture_source_rect_.y = tile_bounds_arg_.height * tile_index_.y;
}

vec2<int> TileMap::tile_index() const {
	return tile_index_;
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

	texture_source_rect_ = {
		.x = 0,
		.y = 0,
		.width = tile_bounds_arg_.width,
		.height = tile_bounds_arg_.height
	};

	set_bounds(texture_source_rect_.bounds<int>());
	toggle_antialiasing(true);
	enable_draw();

	return true;
}

// protected
void TileMap::draw(mat3 const& transform, rgba color) const {
	auto renderer = context().renderer_wref().lock();

	if (!renderer || !texture_) {
		return;
	}

	renderer->draw_texture(*texture_, texture_source_rect_, transform, color);
}

}