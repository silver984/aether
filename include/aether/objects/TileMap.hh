#ifndef __AETHER_OBJECTS_TILEMAP_HH__
#define __AETHER_OBJECTS_TILEMAP_HH__

#include <aether/objects/Sprite.hh>
#include <aether/math/rect.hh>
#include <aether/math/size.hh>
#include <string_view>

namespace ae {

class TileMap : public Sprite {
public:
	TileMap(std::string_view file, size<float> tile_bounds);
	~TileMap() override;

	bool set_texture(std::string_view) = delete;
	void set_texture_wrap(texture_wrap) = delete;
	void set_texture_source_rect(rect<float>) = delete;
	rect<float> texture_source_rect() const = delete;

	[[nodiscard]] std::string_view type() const override;

protected:
	bool setup() override;

	size<float> tile_bounds_arg_;
};

}

#endif