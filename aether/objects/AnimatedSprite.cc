#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <Context.hh>
#include <objects/AnimatedSprite.hh>
#include <raylib.h>
#include <services/Renderer.hh>
#include <services/resource/AnimationRepository.hh>
#include <services/resource/TextureRepository.hh>
#include <util/math.hh>

namespace aether {

AnimatedSprite::AnimatedSprite(Context const& ctx, descriptor desc)
    : NodeIdentity<AnimatedSprite>(ctx)
    , image_file_arg_(desc.image_file)
    , data_file_arg_(desc.data_file)
    , current_subtexture_index_(0)
    , playback_fps_(static_cast<std::uint32_t>(std::max(1, desc.fps)))
    , subtexture_transform_(mat3::identity())
    , subtexture_elapsed_(0.f)
    , animation_was_reset_(false)
    , is_current_animation_looping_(false)
    , is_current_subtexture_rotated_(false)
    , has_antialiasing_arg_(desc.has_antialiasing) {}

AnimatedSprite::~AnimatedSprite() = default;

void AnimatedSprite::toggle_antialiasing(bool val) const {
	if (texture_) {
		SetTextureFilter(*texture_, val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
	}
}

bool AnimatedSprite::play_animation(std::string_view name) {
	if (!data_) {
#ifdef AETHER_VERBOSE_DEBUG
		debuglog("Attempted to play animation with nullptr data");
#endif
		return false;
	}

	if (!data_->contains(name)) {
#ifdef AETHER_VERBOSE_DEBUG
		debuglog("Attempted to play animation not found from data | name: \"{}\"", name);
#endif
		return false;
	}

	// just to avoid string construction when not needed
	if (current_animation_name_ != name) {
		current_animation_name_ = std::string(name);
		animation_was_reset_    = true;
	}

	// reset state
	current_subtexture_index_ = 0;
	subtexture_elapsed_       = 0.f;

	return true;
}

bool AnimatedSprite::play_animation(std::string_view name, animation_options options) {
	if (!play_animation(name)) {
		return false;
	}

	is_current_animation_looping_ = options.loop;

	if (options.fps.has_value()) {
		playback_fps_ = std::max(1, options.fps.value());
	}

	return true;
}

// protected
bool AnimatedSprite::init() {
	texture_ = ctx_.texture_repository.fetch(image_file_arg_);

	if (!texture_) {
#ifdef AETHER_DEBUG
		errorlog("Failed | nullptr texture");
#endif
		return false;
	}

	data_ = ctx_.animation_repository.fetch(data_file_arg_);

	if (!data_) {
#ifdef AETHER_DEBUG
		errorlog("Failed | nullptr data");
#endif
		return false;
	}

	if (data_->empty()) {
#ifdef AETHER_DEBUG
		errorlog("Failed | no valid frames");
#endif
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
	activate();
	update(0.f);
	enable_draw();

	return true;
}

// protected
void AnimatedSprite::update(float dt) {
	if (animation_was_reset_) {
		progress_frame();
		animation_was_reset_ = false;
	}

	float const target_subtexture_time = 1.f / playback_fps_;
	subtexture_elapsed_ += dt;

	// advance a frame
	while (subtexture_elapsed_ >= target_subtexture_time) {
		progress_frame();
		subtexture_elapsed_ -= target_subtexture_time;
	}
}

// protected
void AnimatedSprite::draw(mat3 const& transform, rgba color) {
	if (!texture_) {
		return;
	}

	if (is_current_subtexture_rotated_) {
		mat3 const fix        = mat3::translation({0.f, bounds().height - current_subtexture_offsets_.y});
		mat3 const r          = mat3::rotation(util::degrees_to_radians(-90.f));
		mat3 const t          = mat3::translation(util::switch_sides(-current_subtexture_offsets_));
		subtexture_transform_ = transform * fix * r * t;
	} else {
		mat3 const t          = mat3::translation(-current_subtexture_offsets_);
		subtexture_transform_ = transform * t;
	}

	ctx_.renderer.draw_texture(*texture_, texture_source_rect_, subtexture_transform_, color);
}

// private
void AnimatedSprite::progress_frame() {
	if (!data_ || data_->empty()) {
		return;
	}

	auto const& frames = (*data_)[current_animation_name_].frames;
	set_bounds(calculate_bounds(frames));

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

// private
size<int> AnimatedSprite::calculate_bounds(std::vector<atlas_region> const& frames) const {
	size<int> ret;

	for (auto const& frame : frames) {
		size<int> const lbounds = frame.source_rect.bounds<int>();
		vec2<int> const offsets = util::abs(frame.offsets);

		if (frame.is_rotated) {
			ret = util::max(ret, util::switch_sides(lbounds) + offsets);
			continue;
		}

		ret = util::max(ret, lbounds + offsets);
	}

	return ret;
}

} // namespace aether