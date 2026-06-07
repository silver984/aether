#ifdef AETHER_DEBUG
	#include <debug/log.hh>
#endif
#include <cmath>
#include <context.hh>
#include <objects/sprite.hh>
#include <raylib.h>
#include <renderer.hh>
#include <texture_repository.hh>

namespace aether {

sprite::sprite(context const& ctx, descriptor const& desc)
    : node(ctx)
    , file_arg_(std::string(desc.file))
    , wrap_type_arg_(desc.wrap_type)
    , has_antialiasing_arg_(desc.has_antialiasing) {}

sprite::~sprite() = default;

void sprite::toggle_antialiasing(bool val) const {
	if (texture_) {
		SetTextureFilter(*texture_, val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
	}
}

bool sprite::set_texture(std::string_view file) {
	texture_ = ctx().resource().textures().fetch(file);

	if (!texture_) {
#ifdef AETHER_DEBUG
		errorlog("Failed");
#endif
		return false;
	}

	size<int> const new_bounds = size<int>(texture_->width, texture_->height);
	texture_source_rect_       = rect<float>(0.f, 0.f, (float)new_bounds.width, (float)new_bounds.height);
	set_bounds(new_bounds);

	return true;
}

void sprite::set_texture_wrap(texture_wrap type) {
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

void sprite::set_texture_source_rect(rect<int> val) {
	texture_source_rect_ = static_cast<rect<float>>(val);
}

void sprite::update_bounds() {
	set_bounds(size<int>((int)std::round(texture_source_rect_.width), (int)std::round(texture_source_rect_.height)));
}

rect<int> sprite::texture_source_rect() const {
	return rect<int>((int)std::round(texture_source_rect_.x), (int)std::round(texture_source_rect_.y),
	                 (int)std::round(texture_source_rect_.width), (int)std::round(texture_source_rect_.height));
}

// protected
bool sprite::init() {
	if (!set_texture(file_arg_)) {
#ifdef AETHER_DEBUG
		errorlog("Failed");
#endif
		return false;
	}

	set_texture_wrap(wrap_type_arg_);
	toggle_antialiasing(has_antialiasing_arg_);
	schedule_draw();

	return true;
}

// protected
void sprite::draw(mat3 const& transform, rgba color) {
	if (texture_) {
		ctx().core().fetch_renderer().draw_texture(*texture_, texture_source_rect_, transform, color);
	}
}

} // namespace aether