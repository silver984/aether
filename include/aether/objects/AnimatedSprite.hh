#pragma once
#include <aether/graphic/texture_atlas.hh>
#include <aether/math_types/rect.hh>
#include <aether/objects/Node.hh>
#include <cstddef>
#include <cstdint>

struct Texture;

namespace ae {

class AnimatedSprite : public NodeIdentity<AnimatedSprite> {
	friend class NodeIdentity<AnimatedSprite>;

public:
	AnimatedSprite(Context const& ctx, std::string_view image_file, std::string_view data_file, int fps);
	~AnimatedSprite() override;

	void toggle_antialiasing(bool val) const;
	bool play_anim(std::string_view animation_name, bool should_loop = false, int fps = 0);
	[[nodiscard]] std::vector<std::string> animation_names() const;
	[[nodiscard]] std::string_view current_animation_name() const;
	[[nodiscard]] std::size_t current_subtexture_index() const;
	[[nodiscard]] std::uint32_t playback_fps() const;

protected:
	bool init() override;
	void update(float dt) override;
	void draw(mat3 const& transform, rgba color) override;

private:
	using Node::set_bounds;
	void progress_frame();
	[[nodiscard]] size<int> calculate_bounds(std::vector<texture_atlas::subtexture> const& subtextures) const;

	static constexpr std::string_view TYPE_ = "AnimatedSprite";
	std::shared_ptr<texture_atlas> texture_atlas_;
	std::shared_ptr<Texture> texture_;
	std::string current_animation_name_;
	std::string const image_file_arg_;
	std::string const data_file_arg_;
	std::size_t current_subtexture_index_;
	std::uint32_t playback_fps_;
	rect<int> texture_source_rect_;
	vec2<int> current_subtexture_offsets_;
	mat3 subtexture_transform_;
	float subtexture_elapsed_;
	bool animation_was_reset_;
	bool is_current_animation_looping_;
	bool is_current_subtexture_rotated_;
};

} // namespace ae