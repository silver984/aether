#include <aether/objects/AnimatedSprite.hh>
#include <aether/systems/Resource.hh>
#include <aether/systems/Renderer.hh>
#include <aether/common/log.hh>
#include <raylib.h>

namespace ae {

AnimatedSprite::AnimatedSprite(Context const& ctx, std::string_view path, std::string_view image_format, std::string_view data_format, int fps) :
	Node(ctx),
	frame_elapsed_(0.f),
	cur_frame_index_(0),
	path_arg_(std::string(path)),
	image_format_arg_(std::string(image_format)),
	data_format_arg_(std::string(data_format)),
	fps_arg_(std::max(1, fps))
{}

AnimatedSprite::~AnimatedSprite() = default;

std::string_view AnimatedSprite::type() const {
	return "AnimatedSprite";
}

void AnimatedSprite::toggle_antialiasing(bool val) const {
	if (texture_atlas_ && texture_atlas_->texture) {
		SetTextureFilter(*texture_atlas_->texture, val ? TextureFilter::TEXTURE_FILTER_BILINEAR : TextureFilter::TEXTURE_FILTER_POINT);
	}
}

void AnimatedSprite::play_anim(std::string_view anim_name, int fps, bool should_loop) {
	if (!texture_atlas_ || !texture_atlas_->subtextures.contains(anim_name)) {
		return;
	}

	// just to avoid string construction when not needed
	if (cur_anim_name_ != anim_name) {
		cur_anim_name_ = std::string(anim_name);
	}

	// reset state
	cur_frame_index_ = 0;
	frame_elapsed_ = 0.f;

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
	auto const& first_frame = first_animation->second.begin();
	texture_source_rect_ = first_frame->source_rect;
	cur_frame_offsets_ = first_frame->transform_offset;
	cur_anim_name_ = first_animation->first;

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

	auto const& cur_anim_frames = texture_atlas_->subtextures[cur_anim_name_];
	float const target_frame_time = 1.f / fps_arg_;
	frame_elapsed_ += dt;

	// advance a frame
	while (frame_elapsed_ >= target_frame_time) {
		// TODO: non looping animation
		cur_frame_index_ = (cur_frame_index_ + 1) % cur_anim_frames.size();

		auto const& cur_anim_frame = cur_anim_frames[cur_frame_index_];
		texture_source_rect_ = cur_anim_frame.source_rect;
		cur_frame_offsets_ = cur_anim_frame.transform_offset;

		frame_elapsed_ -= target_frame_time;
	}
}

// protected
void AnimatedSprite::draw(mat3 const& transform, rgba color) const {
	auto renderer = context().renderer_wref().lock();

	if (!renderer || !texture_atlas_ || !texture_atlas_->texture) {
		return;
	}
	
	renderer->draw_texture(*texture_atlas_->texture, texture_source_rect_, transform, color, -cur_frame_offsets_);
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
			avg += vec.source_rect.bounds<int>();
		}
	}

	if (count > 0) {
		return avg / count;
	}

	return {};
}

}