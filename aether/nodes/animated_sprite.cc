#include <animation_repository.hh>
#include <context.hh>
#include <data/texture2d.hh>
#include <debug/log.hh>
#include <nodes/animated_sprite.hh>
#include <renderer.hh>
#include <texture_repository.hh>
#include <util/math.hh>

namespace aether {

animated_sprite::animated_sprite(context const& ctx_, descriptor const& desc)
        : node(ctx_)
        , image_file_arg_(std::string(desc.image_file))
        , data_file_arg_(std::string(desc.data_file))
        , current_subtexture_index_(0)
        , playback_fps_(std::max(1, desc.fps))
        , subtexture_transform_(mat3::identity())
        , subtexture_elapsed_(0.f)
        , animation_was_reset_(false)
        , is_current_animation_looping_(false)
        , is_current_subtexture_rotated_(false)
        , has_antialiasing_arg_(desc.has_antialiasing) {}

animated_sprite::~animated_sprite() = default;

void animated_sprite::toggle_antialiasing(bool val) const {
	SetTextureFilter(texture_->get(), val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
}

bool animated_sprite::play_animation(std::string_view name) {
	if (!data_) {
		AETHER_DEBUGLOG("Attempted to play animation with nullptr data");
		return false;
	}

	if (!data_->contains(name)) {
		AETHER_DEBUGLOG("Attempted to play animation not found from data | name: \"{}\"", name);
		return false;
	}

	// just to avoid string construction when not needed
	if (current_animation_name_ != name) {
		current_animation_name_ = std::string(name);
		animation_was_reset_    = true;
	}

	// reset state
	current_subtexture_index_     = 0;
	subtexture_elapsed_           = 0.f;
	is_current_animation_looping_ = false;

	return true;
}

bool animated_sprite::play_animation(std::string_view name, animation_options options) {
	if (!play_animation(name)) {
		return false;
	}

	is_current_animation_looping_ = options.loop;

	if (options.fps.has_value()) {
		playback_fps_ = std::max(1, options.fps.value());
	}

	return true;
}

bool animated_sprite::init_() {
	auto const& ctx = ctx_();
	texture_        = ctx.textures().fetch(image_file_arg_);

	if (!texture_) {
		AETHER_ERRORLOG("Failed | nullptr texture");
		return false;
	}

	data_ = ctx.animations().fetch(data_file_arg_);

	if (!data_) {
		AETHER_ERRORLOG("Failed | nullptr data");
		return false;
	}

	if (data_->empty()) {
		AETHER_ERRORLOG("Failed | no valid frames");
		return false;
	}

	// set default animation
	auto const first_animation     = data_->begin();
	auto const& first_frame        = first_animation->second.frames.front();
	current_animation_name_        = first_animation->first;
	is_current_subtexture_rotated_ = first_frame.is_rotated;
	texture_source_rect_           = static_cast<rect<float>>(first_frame.source_rect);
	current_subtexture_offsets_    = static_cast<vec2<float>>(first_frame.offsets);
	animation_was_reset_           = true;

	toggle_antialiasing(has_antialiasing_arg_);
	update_(0.f);
	activate();
	schedule_draw();

	return true;
}

void animated_sprite::update_(float dt) {
	if (animation_was_reset_) {
		progress_frame_();
		animation_was_reset_ = false;
	}

	float const target_subtexture_time = 1.f / playback_fps_;
	subtexture_elapsed_ += dt;

	// advance a frame
	while (subtexture_elapsed_ >= target_subtexture_time) {
		progress_frame_();
		subtexture_elapsed_ -= target_subtexture_time;
	}
}

void animated_sprite::draw_(mat3 const& transform, rgba color) {
	if (is_current_subtexture_rotated_) {
		mat3 const fix        = mat3::translation(vec2<float>(0.f, bounds().height - current_subtexture_offsets_.y));
		mat3 const r          = mat3::rotation(util::degrees_to_radians(-90.f));
		mat3 const t          = mat3::translation(util::reverse(-current_subtexture_offsets_));
		subtexture_transform_ = transform * fix * r * t;
	} else {
		mat3 const t          = mat3::translation(-current_subtexture_offsets_);
		subtexture_transform_ = transform * t;
	}

	ctx_().get_renderer().draw_texture(texture_->get(), texture_source_rect_, subtexture_transform_, color);
}

void animated_sprite::progress_frame_() {
	auto const& frames = (*data_)[current_animation_name_].frames;
	set_bounds(calculate_bounds_(frames));

	if (is_current_animation_looping_) {
		current_subtexture_index_ = (current_subtexture_index_ + 1) % frames.size();
	} else {
		current_subtexture_index_ = std::min(current_subtexture_index_ + 1, frames.size() - 1);
	}

	auto const& current_frame      = frames[current_subtexture_index_];
	is_current_subtexture_rotated_ = current_frame.is_rotated;
	texture_source_rect_           = static_cast<rect<float>>(current_frame.source_rect);
	current_subtexture_offsets_    = static_cast<vec2<float>>(current_frame.offsets);
}

size<int> animated_sprite::calculate_bounds_(std::vector<atlas_region> const& frames) const {
	size<int> ret;

	for (auto const& frame : frames) {
		size<int> const source_bounds = frame.source_rect.bounds();
		vec2<int> const offsets       = util::abs(frame.offsets);

		if (frame.is_rotated) {
			ret = util::max(ret, util::reverse(source_bounds) + offsets);
			continue;
		}

		ret = util::max(ret, source_bounds + offsets);
	}

	return ret;
}

} // namespace aether