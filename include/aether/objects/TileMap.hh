#ifndef __AETHER_OBJECTS_TILEMAP_HH__
#define __AETHER_OBJECTS_TILEMAP_HH__

#include <aether/objects/Node.hh>
#include <aether/math/rect.hh>

struct Texture;

namespace ae {

class TileMap : public Node {
public:
	TileMap(Context const& ctx, std::string_view file, size<float> tile_bounds);
	~TileMap() override;

	[[nodiscard]] std::string_view type() const override;
	void toggle_antialiasing(bool val) const;

protected:
	bool init() override;

private:
	std::shared_ptr<Texture> texture_;
	rect<float> texture_source_rect_;
	std::string const file_arg_;
	size<float> const tile_bounds_arg_;
};

}

#endif