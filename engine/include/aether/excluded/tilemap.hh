#pragma once
#include <aether/node/node.hh>
#include <aether/rect.hh>

namespace aether {

class texture2d;

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
	[[nodiscard]] vec2<int> tile_count() const;
	void seek_tile(vec2<int> tile_index);
	[[nodiscard]] vec2<int> tile_index() const;
	// todo: set_texture

protected:
	bool init_() override;
	void draw_(mat3 const& transform, rgba color) override;

private:
	descriptor const desc_;
	strong_ref<texture2d> texture_;
	rect<float> texture_source_rect_;
	vec2<int> tile_index_;
};

} // namespace aether