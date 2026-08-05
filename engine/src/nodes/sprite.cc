#include <cmath>
#include <context.hh>
#include <debug/log.hh>
#include <nodes/sprite.hh>
#include <raylib.h>
#include <renderer.hh>
#include <resources.hh>

namespace aether {

sprite::sprite(context const& ctx, sprite_args const& args) noexcept
        : node(ctx)
        , args_(args) {
}
sprite::~sprite() noexcept = default;

void sprite::toggle_antialiasing(bool val) const {
	SetTextureFilter(*texture_, val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
}

bool sprite::set_texture(std::string_view file) {
	context const& ctx = ctx_();

	if (auto fetched_texture = ctx.textures().load(ctx.aether_resources(), file)) {
		texture_ = fetched_texture;
	} else {
		AETHER_ENGINE_ERRORLOG("Texture is nullptr");
		return false;
	}

	size<int> const new_bounds = size<int>(texture_->width, texture_->height);
	texture_source_rect_       = rect<float>(0.f, 0.f, (float)new_bounds.width, (float)new_bounds.height);
	set_bounds(new_bounds);

	return true;
}

void sprite::set_texture_wrap(texture_wrap type) {
	if (!texture_) {
		return;
	}

	switch (type) {
		using enum texture_wrap;
	case clamp: {
		SetTextureWrap(*texture_, TEXTURE_WRAP_CLAMP);
		break;
	}
	case repeat: {
		SetTextureWrap(*texture_, TEXTURE_WRAP_REPEAT);
		break;
	}
	case mirror_clamp: {
		SetTextureWrap(*texture_, TEXTURE_WRAP_MIRROR_CLAMP);
		break;
	}
	case mirror_repeat: {
		[[fallthrough]];
	}
	default: {
		SetTextureWrap(*texture_, TEXTURE_WRAP_MIRROR_REPEAT);
		break;
	}
	}
}

void sprite::set_texture_source_rect(rect<float> const& val) {
	texture_source_rect_ = val;
}

void sprite::update_bounds() {
	set_bounds(size<int>((int)std::round(texture_source_rect_.width), (int)std::round(texture_source_rect_.height)));
}

rect<float> sprite::texture_source_rect() const {
	return texture_source_rect_;
}

bool sprite::init_() {
	if (!node::init_()) {
		return false;
	}

	if (!set_texture(args_.file)) {
		AETHER_ENGINE_ERRORLOG("Failed to set texture");
		return false;
	}

	set_texture_wrap(args_.wrap_type);
	toggle_antialiasing(args_.has_antialiasing);
	schedule_draw();

	return true;
}

void sprite::draw_(mat3 const& transform, rgba color) {
	node::draw_(transform, color);
	ctx_().renderer().draw_texture(*texture_, texture_source_rect_, transform, color);
}

} // namespace aether