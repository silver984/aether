#ifndef __AETHER_OBJECTS_ANIMATEDSPRITE_HH__
#define __AETHER_OBJECTS_ANIMATEDSPRITE_HH__

#include <aether/graphic/texture_atlas.hh>
#include <aether/math/rect.hh>
#include <aether/objects/Node.hh>
#include <cstddef>

namespace ae {

class AnimatedSprite : public Node {
public:
	AnimatedSprite(Context const& ctx, std::string_view path, std::string_view image_format,
	               std::string_view data_format, int fps);
	~AnimatedSprite() override;

	[[nodiscard]] std::string_view type() const override;
	void toggle_antialiasing(bool val) const;
	void play_anim(std::string_view anim_name, bool should_loop = false, int fps = 0);

protected:
	bool init() override;
	void update(float dt) override;
	void draw(mat3 const& transform, rgba color) const override;

private:
	[[nodiscard]] size<int> calculate_bounds() const;

	std::shared_ptr<texture_atlas> texture_atlas_;
	rect<int> texture_source_rect_;
	vec2<int> cur_frame_offsets_;
	float frame_elapsed_;
	size_t cur_frame_index_;
	bool is_cur_anim_looping_;
	bool is_cur_frame_rotated_;
	std::string cur_anim_name_;
	std::string const path_arg_;
	std::string const image_format_arg_;
	std::string const data_format_arg_;
	int fps_arg_; // purposefully modifiable
};

} // namespace ae

#endif