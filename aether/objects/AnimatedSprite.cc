#include <aether/common/log.hh>
#include <aether/math/util.hh>
#include <aether/objects/AnimatedSprite.hh>
#include <aether/systems/Renderer.hh>
#include <aether/systems/Resource.hh>
#include <raylib.h>

namespace ae {

AnimatedSprite::AnimatedSprite(Context const& ctx, std::string_view path, std::string_view image_format,
                               std::string_view data_format, int fps)
    : Node(ctx), subtexture_elapsed_(0.f), current_subtexture_index_(0), animation_reset_(false),
      is_current_animation_looping_(false), is_current_subtexture_rotated_(false),
      is_subtexture_transform_dirty_(false), subtexture_transform_(mat3::identity()), path_arg_(std::string(path)),
      image_format_arg_(std::string(image_format)), data_format_arg_(std::string(data_format)),
      fps_arg_(std::max(1, fps)) {}

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

void AnimatedSprite::play_anim(std::string_view anim_name, bool should_loop, int fps) {
	if (!texture_atlas_ || !texture_atlas_->subtextures.contains(anim_name)) {
		return;
	}

	// just to avoid string construction when not needed
	if (current_anim_name_ != anim_name) {
		current_anim_name_ = std::string(anim_name);
	}

	// reset state
	current_subtexture_index_     = 0;
	subtexture_elapsed_           = 0.f;
	is_current_animation_looping_ = should_loop;

	// change fps if arg is not default
	if (fps != 0) {
		fps_arg_ = std::max(1, fps);
	}

	animation_reset_ = true;
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
	auto const& first_subtexture   = first_animation->second.begin();
	current_anim_name_             = first_animation->first;
	texture_source_rect_           = first_subtexture->source_rect;
	current_subtexture_offsets_    = first_subtexture->offsets;
	is_current_subtexture_rotated_ = first_subtexture->is_rotated;
	is_subtexture_transform_dirty_ = true;

	toggle_antialiasing(true);
	set_bounds(calculate_bounds(first_animation->second));
	activate();
	enable_draw();

	return true;
}

// protected
void AnimatedSprite::update(float dt) {
	if (!texture_atlas_) {
		return;
	}

	float const target_subtexture_time = 1.f / fps_arg_;
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

	if (is_subtexture_transform_dirty_) {
		mat3 t                         = mat3::translation(-(static_cast<vec2<float>>(current_subtexture_offsets_)));
		subtexture_transform_          = transform * t;
		is_subtexture_transform_dirty_ = false;
	}

	renderer->draw_rect(bounds(), transform, {255, 0, 0, 255});
	renderer->draw_texture(*texture_atlas_->texture, texture_source_rect_, subtexture_transform_, color);
}

// private
void AnimatedSprite::progress_frame() {
	auto const& current_animation = texture_atlas_->subtextures[current_anim_name_];

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

	if (is_current_subtexture_rotated_ != current_subtexture.is_rotated) {
		is_current_subtexture_rotated_ = current_subtexture.is_rotated;
		is_subtexture_transform_dirty_ = true;
	}

	if (current_subtexture_offsets_ != current_subtexture.offsets) {
		current_subtexture_offsets_    = current_subtexture.offsets;
		is_subtexture_transform_dirty_ = true;
	}
}

// private
size<int> AnimatedSprite::calculate_bounds(std::vector<texture_atlas::subtexture> const& animation) const {
	if (!texture_atlas_) {
		return {};
	}

	size<int> ret;

	for (auto const& subtexture : animation) {
		ret = math::max(ret, subtexture.source_rect.bounds<int>() - subtexture.offsets);
	}

	return ret;
}

} // namespace ae