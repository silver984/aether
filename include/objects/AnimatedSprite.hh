#pragma once
#include <cstddef>
#include <cstdint>
#include <graphic/animation_map.hh>
#include <math_types/rect.hh>
#include <objects/Node.hh>
#include <optional>

struct Texture;

namespace aether {

class AnimatedSprite : public NodeIdentity<AnimatedSprite> {
	friend class NodeIdentity<AnimatedSprite>;

public:
	struct descriptor final {
		std::string_view image_file;
		std::string_view data_file;
		int fps;
		bool has_antialiasing = true;
	};

	struct animation_options final {
		std::optional<int> fps = std::nullopt;
		bool loop              = false;
	};

	AnimatedSprite(Context const& ctx, descriptor desc);
	~AnimatedSprite() override;

	void toggle_antialiasing(bool val) const;
	bool play_animation(std::string_view name);
	bool play_animation(std::string_view name, animation_options options);

protected:
	bool init() override;
	void update(float dt) override;
	void draw(mat3 const& transform, rgba color) override;

private:
	using Node::set_bounds;
	void progress_frame();
	[[nodiscard]] size<int> calculate_bounds(std::vector<atlas_region> const& frames) const;

	static constexpr std::string_view TYPE_ = "AnimatedSprite";
	std::shared_ptr<animation_map> data_;
	std::shared_ptr<Texture> texture_;
	std::string current_animation_name_;
	std::string_view const image_file_arg_;
	std::string_view const data_file_arg_;
	std::size_t current_subtexture_index_;
	std::uint32_t playback_fps_;
	rect<float> texture_source_rect_;
	vec2<float> current_subtexture_offsets_;
	mat3 subtexture_transform_;
	float subtexture_elapsed_;
	bool animation_was_reset_;
	bool is_current_animation_looping_;
	bool is_current_subtexture_rotated_;
	bool const has_antialiasing_arg_;
};

} // namespace ae