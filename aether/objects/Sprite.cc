#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <Context.hh>
#include <cmath>
#include <objects/Sprite.hh>
#include <raylib.h>
#include <services/Renderer.hh>
#include <services/resource/TextureRepository.hh>

namespace {

int round_int_cast(float val) {
	return static_cast<int>(std::round(val));
}

} // namespace

namespace aether {

Sprite::Sprite(Context const& ctx, descriptor desc)
    : NodeIdentity<Sprite>(ctx)
    , file_arg_(std::string(desc.file))
    , wrap_type_arg_(desc.wrap_type)
    , has_antialiasing_arg_(desc.has_antialiasing) {}

Sprite::~Sprite() = default;

void Sprite::toggle_antialiasing(bool val) const {
	if (texture_) {
		SetTextureFilter(*texture_, val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
	}
}

bool Sprite::set_texture(std::string_view file) {
	texture_ = ctx_.texture_repo.fetch(file);

	if (!texture_) {
#ifdef AETHER_DEBUG
		errorlog("Failed");
#endif
		return false;
	}

	size<int> new_bounds = {texture_->width, texture_->height};
	texture_source_rect_ = {0.f, 0.f, static_cast<float>(new_bounds.width), static_cast<float>(new_bounds.height)};
	set_bounds(new_bounds);

	return true;
}

void Sprite::set_texture_wrap(texture_wrap type) {
	if (!texture_) {
#ifdef AETHER_VERBOSE_DEBUG
		debuglog("Attempted to set texture wrap with nullptr texture");
#endif
		return;
	}

	switch (type) {
		using enum texture_wrap;
	case clamp: SetTextureWrap(*texture_, TEXTURE_WRAP_CLAMP); break;
	case repeat: SetTextureWrap(*texture_, TEXTURE_WRAP_REPEAT); break;
	case mirror_clamp: SetTextureWrap(*texture_, TEXTURE_WRAP_MIRROR_CLAMP); break;
	case mirror_repeat: SetTextureWrap(*texture_, TEXTURE_WRAP_MIRROR_REPEAT); break;
	}
}

void Sprite::set_texture_source_rect(rect<int> val) {
	texture_source_rect_ = static_cast<rect<float>>(val);
}

void Sprite::update_bounds() {
	set_bounds({round_int_cast(texture_source_rect_.width), round_int_cast(texture_source_rect_.height)});
}

rect<int> Sprite::texture_source_rect() const {
	return {round_int_cast(texture_source_rect_.x), round_int_cast(texture_source_rect_.y),
	        round_int_cast(texture_source_rect_.width), round_int_cast(texture_source_rect_.height)};
}

// protected
bool Sprite::init() {
	if (!set_texture(file_arg_)) {
#ifdef AETHER_DEBUG
		errorlog("Failed");
#endif
		return false;
	}

	set_texture_wrap(wrap_type_arg_);
	toggle_antialiasing(has_antialiasing_arg_);
	enable_draw();

	return true;
}

// protected
void Sprite::draw(mat3 const& transform, rgba color) {
	if (texture_) {
		ctx_.renderer.draw_texture(*texture_, texture_source_rect_, transform, color);
	}
}

} // namespace aether