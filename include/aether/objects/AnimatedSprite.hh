#ifndef __AETHER_OBJECTS_ANIMATEDSPRITE_HH__
#define __AETHER_OBJECTS_ANIMATEDSPRITE_HH__

#include <aether/graphic/texture_atlas.hh>
#include <aether/math/rect.hh>
#include <aether/objects/Node.hh>
#include <cstddef>
#include <cstdint>

namespace ae {

class Renderer;

class AnimatedSprite : public Node {
public:
	AnimatedSprite(Context const& ctx, std::string_view path, std::string_view image_format,
	               std::string_view data_format, int fps);
	~AnimatedSprite() override;

	[[nodiscard]] std::string_view type() const override;
	void toggle_antialiasing(bool val) const;
	void play_anim(std::string_view animation_name, bool should_loop = false, int fps = 0);
	[[nodiscard]] std::vector<std::string> animation_names() const;
	[[nodiscard]] std::string_view current_animation_name() const;
	[[nodiscard]] size_t current_subtexture_index() const;
	[[nodiscard]] uint32_t playback_fps() const;

protected:
	bool init() override;
	void update(float dt) override;
	void draw(mat3 const& transform, rgba color) override;

private:
	using Node::set_bounds;
	void progress_frame();
	[[nodiscard]] size<int> calculate_bounds(std::vector<texture_atlas::subtexture> const& subtextures) const;

	std::shared_ptr<texture_atlas> texture_atlas_;
	rect<int> texture_source_rect_;
	vec2<int> current_subtexture_offsets_;
	float subtexture_elapsed_;
	size_t current_subtexture_index_;
	bool animation_reset_;
	bool is_current_animation_looping_;
	bool is_current_subtexture_rotated_;
	mat3 subtexture_transform_;
	std::string current_animation_name_;
	std::string const path_arg_;
	std::string const image_format_arg_;
	std::string const data_format_arg_;
	uint32_t playback_fps_;
};

} // namespace ae

#endif