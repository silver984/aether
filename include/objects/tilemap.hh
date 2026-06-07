#pragma once
#include <cstdint>
#include <math/rect.hh>
#include <objects/node.hh>

struct Texture;
using rltexture = Texture;

namespace aether {

class tilemap : public node {
public:
	struct descriptor final {
		std::string_view file;
		size<int> tile_bounds;
		bool has_antialiasing = true;
	};

	tilemap(context const& ctx, descriptor const& desc);
	~tilemap() override;

	void toggle_antialiasing(bool val) const;
	[[nodiscard]] vec2<uint32_t> tile_count() const;
	void seek_tile(vec2<int> tile_index);
	[[nodiscard]] vec2<uint32_t> tile_index() const;
	// todo: set_texture

protected:
	bool init() override;
	void draw(mat3 const& transform, rgba color) override;

private:
	std::shared_ptr<rltexture> texture_;
	std::string const file_arg_;
	rect<float> texture_source_rect_;
	vec2<uint32_t> tile_index_;
	size<uint32_t> const tile_bounds_arg_;
	bool const has_antialiasing_;
};

} // namespace aether