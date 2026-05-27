#pragma once
#include <aether/math_types/rect.hh>
#include <aether/objects/Node.hh>
#include <cstdint>

struct Texture;

namespace ae {

class TileMap : public Node {
public:
	TileMap(Context const& ctx, std::string_view file, size<int> tile_bounds);
	~TileMap() override;

	[[nodiscard]] std::string_view type() const override;
	void toggle_antialiasing(bool val) const;
	[[nodiscard]] vec2<std::uint32_t> tile_count() const;
	void seek_tile(vec2<int> tile_index);
	[[nodiscard]] vec2<std::uint32_t> tile_index() const;
	// TODO: set_texture

protected:
	bool init() override;
	void draw(mat3 const& transform, rgba color) override;

private:
	std::shared_ptr<Texture> texture_;
	rect<int> texture_source_rect_;
	vec2<std::uint32_t> tile_index_;
	std::string const file_arg_;
	size<std::uint32_t> const tile_bounds_arg_;
};

} // namespace ae