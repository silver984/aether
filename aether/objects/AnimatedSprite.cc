#include <aether/debug/log.hh>
#include <aether/objects/AnimatedSprite.hh>
#include <aether/systems/Renderer.hh>
#include <aether/systems/Resource.hh>
#include <aether/util/math.hh>
#include <raylib.h>

namespace ae {

AnimatedSprite::AnimatedSprite(Context const& ctx, std::string_view path, std::string_view image_format,
                               std::string_view data_format, int fps)
    : Node(ctx), subtexture_elapsed_(0.f), current_subtexture_index_(0), animation_reset_(false),
      is_current_animation_looping_(false), is_current_subtexture_rotated_(false),
      subtexture_transform_(mat3::identity()), path_arg_(std::string(path)),
      image_format_arg_(std::string(image_format)), data_format_arg_(std::string(data_format)),
      playback_fps_(std::max(1, fps)) {}

AnimatedSprite::~AnimatedSprite() = default;

std::string_view AnimatedSprite::type() const {
	return "AnimatedSprite";
}

void AnimatedSprite::toggle_antialiasing(bool val) const {
	if (texture_atlas_ && texture_atlas_->texture) {
		SetTextureFilter(*texture_atlas_->texture,
		                 val ? TextureFilter::TEXTURE_FILTER_BILINEAR : TextureFilter::TEXTURE_FILTER_POINT);
	}
}

void AnimatedSprite::play_anim(std::string_view animation_name, bool should_loop, int fps) {
	if (!texture_atlas_) {
		debuglog("Attempted to play animation with nullptr texture atlas");
		return;
	}

	if (!texture_atlas_->subtextures.contains(animation_name)) {
		debuglog("Attempted to play animation not found from texture atlas | animation name: \"{}\"", animation_name);
		return;
	}

	// just to avoid string construction when not needed
	if (current_animation_name_ != animation_name) {
		current_animation_name_ = std::string(animation_name);
		animation_reset_        = true;
	}

	// reset state
	current_subtexture_index_     = 0;
	subtexture_elapsed_           = 0.f;
	is_current_animation_looping_ = should_loop;

	// change fps if arg is not default
	if (fps != 0) {
		playback_fps_ = std::max(1, fps);
	}
}

std::vector<std::string> AnimatedSprite::animation_names() const {
	if (!texture_atlas_ || texture_atlas_->subtextures.empty()) {
		return {};
	}

	std::vector<std::string> animation_names;
	animation_names.reserve(texture_atlas_->subtextures.size());

	for (auto const& [name, _] : texture_atlas_->subtextures) {
		animation_names.emplace_back(name);
	}

	return animation_names;
}

std::string_view AnimatedSprite::current_animation_name() const {
	return current_animation_name_;
}

size_t AnimatedSprite::current_subtexture_index() const {
	return current_subtexture_index_;
}

uint32_t AnimatedSprite::playback_fps() const {
	return playback_fps_;
}

// protected
bool AnimatedSprite::init() {
	auto resource = context().resource_wref().lock();

	if (!resource) {
		errorlog("Can't reference resource system");
		return false;
	}

	texture_atlas_ = resource->load_shared_texture_atlas(path_arg_, image_format_arg_, data_format_arg_);

	if (!texture_atlas_) {
		errorlog("Failed");
		return false;
	}

	// set default animation
	auto const first_animation     = texture_atlas_->subtextures.begin();
	auto const first_subtextureset = first_animation->second;
	auto const& first_subtexture   = first_subtextureset.front();
	current_animation_name_        = first_animation->first;
	texture_source_rect_           = first_subtexture.source_rect;
	current_subtexture_offsets_    = first_subtexture.offsets;
	is_current_subtexture_rotated_ = first_subtexture.is_rotated;

	toggle_antialiasing(true);
	set_bounds(calculate_bounds(first_subtextureset));
	activate();
	enable_draw();

	return true;
}

// protected
void AnimatedSprite::update(float dt) {
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
	auto renderer = context().renderer_wref().lock();

	if (!renderer || !texture_atlas_ || !texture_atlas_->texture) {
		return;
	}

	if (is_current_subtexture_rotated_) {
		mat3 const fix = mat3::translation({0.f, static_cast<float>(bounds().height - current_subtexture_offsets_.y)});
		mat3 const r   = mat3::rotation(math::degrees_to_radians(-90.f));
		mat3 const t = mat3::translation(-(static_cast<vec2<float>>(math::switch_sides(current_subtexture_offsets_))));
		subtexture_transform_ = transform * fix * r * t;
	} else {
		mat3 const t          = mat3::translation(-(static_cast<vec2<float>>(current_subtexture_offsets_)));
		subtexture_transform_ = transform * t;
	}

	renderer->draw_texture(*texture_atlas_->texture, texture_source_rect_, subtexture_transform_, color);
}

// private
void AnimatedSprite::progress_frame() {
	if (!texture_atlas_ || texture_atlas_->subtextures.empty()) {
		return;
	}

	auto const& current_animation = texture_atlas_->subtextures[current_animation_name_];

	if (animation_reset_) {
		set_bounds(calculate_bounds(current_animation));
		animation_reset_ = false;
	}

	if (is_current_animation_looping_) {
		current_subtexture_index_ = (current_subtexture_index_ + 1) % current_animation.size();
	} else {
		current_subtexture_index_ = std::min(current_subtexture_index_ + 1, current_animation.size() - 1);
	}

	auto const& current_subtexture = current_animation[current_subtexture_index_];
	texture_source_rect_           = current_subtexture.source_rect;
	is_current_subtexture_rotated_ = current_subtexture.is_rotated;
	current_subtexture_offsets_    = current_subtexture.offsets;
}

// private
size<int> AnimatedSprite::calculate_bounds(std::vector<texture_atlas::subtexture> const& subtextures) const {
	size<int> ret;

	for (auto const& subtexture : subtextures) {
		if (subtexture.is_rotated) {
			ret = math::max(ret,
			                math::switch_sides(subtexture.source_rect.bounds<int>()) + math::abs(subtexture.offsets));
			continue;
		}

		ret = math::max(ret, subtexture.source_rect.bounds<int>() + math::abs(subtexture.offsets));
	}

	return ret;
}

} // namespace ae