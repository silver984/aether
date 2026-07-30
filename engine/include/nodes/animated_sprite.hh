#pragma once
#include <cstddef>
#include <data/animation_map.hh>
#include <math/rect.hh>
#include <nodes/node.hh>
#include <optional>

struct Texture;

namespace aether {

class animated_sprite : public node {
public:
	struct descriptor final {
		std::string_view imagefile;
		std::string_view datafile;
		int fps;
		bool has_antialiasing = true;
	};

	struct animation_options final {
		std::optional<int> fps = std::nullopt;
		bool loop              = false;
	};

	animated_sprite(context const& ctx, descriptor const& desc);
	~animated_sprite() override;

	void toggle_antialiasing(bool val) const;
	bool play_animation(std::string_view name);
	bool play_animation(std::string_view name, animation_options const& options);

protected:
	bool init_() override;
	void update_(float dt) override;
	void draw_(mat3 const& transform, rgba color) override;

private:
	void progress_frame_();
	[[nodiscard]] size<int> calculate_bounds_(std::vector<atlas_region> const& frames) const;

	descriptor const desc_;
	strong_ref<animation_map> data_;
	strong_ref<Texture> texture_;
	std::string current_animation_name_;
	size_t current_subtexture_index_;
	int playback_fps_;
	rect<float> texture_source_rect_;
	vec2<float> current_subtexture_offsets_;
	mat3 subtexture_transform_;
	float subtexture_elapsed_;
	bool animation_was_reset_;
	bool is_current_animation_looping_;
	bool is_current_subtexture_rotated_;
};

} // namespace aether