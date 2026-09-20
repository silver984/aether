#include <aether/context.hh>
#include <aether/log.hh>
#include <aether/renderer.hh>
#include <aether/resources.hh>
#include <aether/sprite.hh>
#include <aether/zip_archive.hh>
#include <cmath>
#include <raylib.h>

namespace aether {

sprite::sprite(context const& ctx, sprite_args const& args)
        : node(ctx)
        , args_(args) {}
sprite::~sprite() = default;

void sprite::toggle_antialiasing(bool val) const { SetTextureFilter(*texture_, val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT); }

bool sprite::set_texture(zip_archive const& pak, std::string_view file) {
	if (auto fetched_texture = this->ctx_.textures->load(pak, file)) {
		texture_ = fetched_texture;
	} else {
		ae_error("Requested texture is nullptr");
		return false;
	}

	float w = (float)texture_->width;
	float h = (float)texture_->height;
	set_texture_source_rect(rect<float>(0.f, 0.f, w, h));

	return true;
}

void sprite::set_texture_wrap(texture_wrap type) {
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
	int w                = (int)std::round(texture_source_rect_.width);
	int h                = (int)std::round(texture_source_rect_.height);
	this->set_bounds(size<int>(w, h));
}

rect<float> sprite::texture_source_rect() const { return texture_source_rect_; }

bool sprite::init_() {
	if (!node::init_()) {
		return false;
	}

	this->schedule_draw();

	if (!set_texture(args_.pak, args_.file)) {
		ae_warn("Failed to set texture, using fallback texture instead");
		use_fallback_texture_();
		return true;
	}

	set_texture_wrap(args_.wrap_type);
	toggle_antialiasing(args_.has_antialiasing);

	return true;
}

void sprite::draw_(mat3 const& transform, rgba color) {
	node::draw_(transform, color);
	this->ctx_.renderer->draw_texture(*texture_, texture_source_rect_, transform, color);
}

void sprite::use_fallback_texture_() {
	// assuming that the fallback texture always exists
	(void)set_texture(zip_archive("aether.pak"), "null");
	set_texture_source_rect(rect<float>(0.f, 0.f, 100.f, 100.f));
	set_texture_wrap(texture_wrap::repeat);
}

} // namespace aether