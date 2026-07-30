#pragma once
#include <math/rect.hh>
#include <nodes/node.hh>

struct Texture;

namespace aether {

enum class texture_wrap : int {
	clamp,
	repeat,
	mirror_clamp,
	mirror_repeat
};

struct sprite_args final {
	std::string_view file;
	texture_wrap wrap_type = texture_wrap::clamp;
	bool has_antialiasing  = true;
};

class sprite : public node {
public:
	sprite(context const& ctx, sprite_args const& desc) noexcept;
	~sprite() noexcept override;
	void toggle_antialiasing(bool val) const;
	bool set_texture(std::string_view file);
	void set_texture_wrap(texture_wrap type);
	void set_texture_source_rect(rect<float> const& val); // todo: pos, bounds, x, y, width, height
	void update_bounds();
	[[nodiscard]] rect<float> texture_source_rect() const;

protected:
	bool init_() override;
	void draw_(mat3 const& transform, rgba color) override;

private:
	sprite_args const args_;
	strong_ref<Texture> texture_;
	rect<float> texture_source_rect_;
};

} // namespace aether