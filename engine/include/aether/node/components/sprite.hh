#pragma once
#include <aether/node/components/component.hh>
#include <aether/rect.hh>
#include <optional>
#include <string_view>

struct Texture;

namespace aether {

class zip_archive;

enum class texture_wrap : uint8_t {
	clamp,
	repeat,
	mirror_clamp,
	mirror_repeat
};

struct sprite_args final {
	zip_archive const& pak;
	std::optional<std::string_view> file = std::nullopt;
	texture_wrap wrap_type               = texture_wrap::repeat;
	bool antialiasing                    = true;
};

// this component adds the transform component if the node doesnt have it yet
class sprite final : public node_component {
public:
	sprite(context const& ctx, strong_ref<node> n, sprite_args const& args) noexcept;

	void set_antialiasing(bool val) noexcept;

	bool set_texture(zip_archive const& pak, std::string_view file) noexcept;
	void set_texture_wrap(texture_wrap wrap_type) noexcept;
	void set_texture_source_rect(rect<float> const& val, bool update_bounds = true) noexcept;

	[[nodiscard]] inline weak_ref<Texture> texture() const noexcept { return texture_; }

protected:
	bool init_() noexcept override;
	void draw_() noexcept override;

private:
	void use_fallback_texture_() noexcept;

	unique_ref<sprite_args> maybe_args_;
	strong_ref<Texture> texture_;
	rect<float> texture_source_rect_;
};

} // namespace aether