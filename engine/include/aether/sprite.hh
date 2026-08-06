#pragma once
#include <aether/node.hh>
#include <aether/rect.hh>
#include <optional>

struct Texture;

namespace aether {

enum class texture_wrap : int {
	clamp,
	repeat,
	mirror_clamp,
	mirror_repeat
};

struct sprite_args final {
	std::optional<std::string_view> file = std::nullopt;
	texture_wrap wrap_type               = texture_wrap::clamp;
	bool has_antialiasing                = true;
};

class sprite : public node {
public:
	sprite(context const& ctx, sprite_args const& args) noexcept;
	~sprite() noexcept override;
	void toggle_antialiasing(bool val) const;
	bool set_texture(std::string_view file);
	void set_texture_wrap(texture_wrap type);
	void set_texture_source_rect(rect<float> const& val); // todo: pos, bounds, x, y, width, height
	[[nodiscard]] rect<float> texture_source_rect() const;

protected:
	bool init_() override;
	void draw_(mat3 const& transform, rgba color) override;

private:
	void use_fallback_texture_();

	sprite_args const args_;
	strong_ref<Texture> texture_;
	rect<float> texture_source_rect_;
};

} // namespace aether