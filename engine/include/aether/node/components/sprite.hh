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
	sprite(context const& ctx, strong_ref<node> n, sprite_args const& args);

	void set_antialiasing(bool val);

	bool set_texture(zip_archive const& pak, std::string_view file);
	void set_texture_wrap(texture_wrap wrap_type);
	void set_texture_source_rect(rect<float> const& val, bool update_bounds = true);

	[[nodiscard]] weak_ref<Texture> texture() const { return texture_; }

protected:
	bool init_() override;
	void draw_() override;

private:
	void use_fallback_texture_();

	unique_ref<sprite_args> maybe_args_;
	strong_ref<Texture> texture_;
	rect<float> texture_source_rect_;
};

} // namespace aether