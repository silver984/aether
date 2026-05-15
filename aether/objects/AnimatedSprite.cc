#include <aether/common/log.hh>
#include <aether/math/util.hh>
#include <aether/objects/AnimatedSprite.hh>
#include <aether/systems/Renderer.hh>
#include <aether/systems/Resource.hh>
#include <raylib.h>

namespace ae {

AnimatedSprite::AnimatedSprite(Context const& ctx, std::string_view path, std::string_view image_format,
                               std::string_view data_format, int fps)
    : Node(ctx), frame_elapsed_(0.f), cur_frame_index_(0), is_cur_anim_looping_(false), is_cur_frame_rotated_(false),
      is_frame_transform_dirty_(false), frame_transform_(mat3::identity()), path_arg_(std::string(path)),
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
	if (cur_anim_name_ != anim_name) {
		cur_anim_name_ = std::string(anim_name);
	}

	// reset state
	cur_frame_index_     = 0;
	frame_elapsed_       = 0.f;
	is_cur_anim_looping_ = should_loop;

	// change fps if arg is not default
	if (fps != 0) {
		fps_arg_ = std::max(1, fps);
	}
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
	auto const first_animation = texture_atlas_->subtextures.begin();
	auto const& first_frame    = first_animation->second.begin();
	cur_anim_name_             = first_animation->first;
	texture_source_rect_       = first_frame->source_rect;
	cur_frame_offsets_         = first_frame->offsets;
	is_cur_frame_rotated_      = first_frame->is_rotated;
	is_frame_transform_dirty_  = true;

	toggle_antialiasing(true);
	set_bounds(calculate_bounds());
	activate();
	enable_draw();

	return true;
}

// protected
void AnimatedSprite::update(float dt) {
	if (!texture_atlas_) {
		return;
	}

	float const target_frame_time = 1.f / fps_arg_;
	frame_elapsed_ += dt;

	// advance a frame
	while (frame_elapsed_ >= target_frame_time) {
		progress_frame();
		frame_elapsed_ -= target_frame_time;
	}
}

// protected
void AnimatedSprite::draw(mat3 const& transform, rgba color) {
	auto renderer = context().renderer_wref().lock();

	if (!renderer || !texture_atlas_ || !texture_atlas_->texture) {
		return;
	}

	if (is_frame_transform_dirty_) {
		mat3 t = mat3::translation(-static_cast<vec2<float>>(cur_frame_offsets_));

		if (is_cur_frame_rotated_) {
			mat3 r           = mat3::rotation(math::degrees_to_radians(-90.f));
			frame_transform_ = transform * t * r;
		} else {
			frame_transform_ = transform * t;
		}

		is_frame_transform_dirty_ = false;
	}

	renderer->draw_texture(*texture_atlas_->texture, texture_source_rect_, frame_transform_, color);
}

// private
void AnimatedSprite::progress_frame() {
	auto const& cur_anim_frames = texture_atlas_->subtextures[cur_anim_name_];

	if (is_cur_anim_looping_) {
		cur_frame_index_ = (cur_frame_index_ + 1) % cur_anim_frames.size();
	} else {
		cur_frame_index_ = std::min(cur_frame_index_ + 1, cur_anim_frames.size() - 1);
	}

	auto const& cur_anim_frame = cur_anim_frames[cur_frame_index_];
	texture_source_rect_       = cur_anim_frame.source_rect;

	if (is_cur_frame_rotated_ != cur_anim_frame.is_rotated) {
		is_cur_frame_rotated_     = cur_anim_frame.is_rotated;
		is_frame_transform_dirty_ = true;
	}

	if (cur_frame_offsets_ != cur_anim_frame.offsets) {
		cur_frame_offsets_        = cur_anim_frame.offsets;
		is_frame_transform_dirty_ = true;
	}
}

// private
size<int> AnimatedSprite::calculate_bounds() const {
	if (!texture_atlas_) {
		return {};
	}

	size<int> avg;
	size_t count = 0;

	for (auto const& [_, second] : texture_atlas_->subtextures) {
		count += second.size();

		for (auto const& vec : second) {
			avg += {vec.is_rotated ? vec.source_rect.height : vec.source_rect.width,
			        vec.is_rotated ? vec.source_rect.width : vec.source_rect.height};
		}
	}

	if (count > 0) {
		return avg / count;
	}

	return {};
}

} // namespace ae