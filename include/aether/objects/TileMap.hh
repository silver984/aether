#ifndef __AETHER_OBJECTS_TILEMAP_HH__
#define __AETHER_OBJECTS_TILEMAP_HH__

#include <aether/objects/Node.hh>
#include <aether/math/rect.hh>

struct Texture;

namespace ae {

class TileMap : public Node {
public:
	TileMap(Context const& ctx, std::string_view file, size<int> tile_bounds);
	~TileMap() override;

	[[nodiscard]] std::string_view type() const override;
	void toggle_antialiasing(bool val) const;
	[[nodiscard]] vec2<int> tile_count() const;
	void seek_tile(vec2<int> tile_index);
	[[nodiscard]] vec2<int> tile_index() const;

protected:
	bool init() override;
	void draw(mat3 const& transform, rgba color) const override;

private:
	std::shared_ptr<Texture> texture_;
	rect<int> texture_source_rect_;
	vec2<int> tile_index_;
	std::string const file_arg_;
	size<int> const tile_bounds_arg_;
};

}

#endif