#pragma once
#include <aether/math_types/rect.hh>
#include <aether/objects/Node.hh>
#include <cstdint>

struct Texture;

namespace ae {

class TileMap : public NodeIdentity<TileMap> {
	friend class NodeIdentity<TileMap>;

public:
	struct descriptor final {
		std::string_view file;
		size<int> tile_bounds;
	};

	TileMap(Context const& ctx, descriptor desc);
	~TileMap() override;

	void toggle_antialiasing(bool val) const;
	[[nodiscard]] vec2<std::uint32_t> tile_count() const;
	void seek_tile(vec2<int> tile_index);
	[[nodiscard]] vec2<std::uint32_t> tile_index() const;
	// TODO: set_texture

protected:
	bool init() override;
	void draw(mat3 const& transform, rgba color) override;

private:
	static constexpr std::string_view TYPE_ = "TileMap";
	std::shared_ptr<Texture> texture_;
	std::string const file_arg_;
	rect<float> texture_source_rect_;
	vec2<std::uint32_t> tile_index_;
	size<std::uint32_t> const tile_bounds_arg_;
};

} // namespace ae