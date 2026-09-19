#pragma once
#include <aether/node/components/component.hh>
#include <aether/node/components/transform.hh>
#include <aether/rect.hh>

#include <string_view>

struct Texture;

namespace aether {

class zip_archive;
class transform;

enum class texture_wrap : uint8_t {
	clamp,
	repeat,
	mirror_clamp,
	mirror_repeat
};

class sprite final : public node_component {
public:
	using dependencies = node_component_list<transform>;
	using node_component::node_component;

	void set_antialiasing(bool val);
	bool set_texture(zip_archive const& pak, std::string_view file);
	void set_texture_wrap(texture_wrap wrap_type);
	void set_texture_source_rect(rect<float> const& val) { texture_source_rect_ = val; }

	bool update_transform_bounds();

	[[nodiscard]] weak_ref<Texture> texture() const { return texture_; }

protected:
	void draw_() override;

private:
	strong_ref<Texture> texture_;
	rect<float> texture_source_rect_;
};

} // namespace aether