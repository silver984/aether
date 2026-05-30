#include <aether/debug/log.hh>
#include <aether/objects/Sprite.hh>
#include <aether/systems/Renderer.hh>
#include <aether/systems/repos/TextureRepo.hh>
#include <raylib.h>

namespace ae {

Sprite::Sprite(Context const& ctx, std::string_view file) : NodeIdentity<Sprite>(ctx), file_arg_(std::string(file)) {}
Sprite::~Sprite() = default;

void Sprite::toggle_antialiasing(bool val) const {
	if (texture_) {
		using enum TextureFilter;
		SetTextureFilter(*texture_, val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
	}
}

bool Sprite::set_texture(std::string_view file) {
	texture_ = ctx_.texture_repo.fetch(file);

	if (!texture_) {
		errorlog("Failed");
		return false;
	}

	size<int> new_bounds = {texture_->width, texture_->height};
	texture_source_rect_ = {0, 0, new_bounds.width, new_bounds.height};
	set_bounds(new_bounds);

	return true;
}

void Sprite::set_texture_wrap(texture_wrap type) {
	if (!texture_) {
		debuglog("Attempted to set texture wrap with nullptr texture");
		return;
	}

	switch (type) {
		using enum texture_wrap;
		using enum TextureWrap;
	case clamp: SetTextureWrap(*texture_, TEXTURE_WRAP_CLAMP); break;
	case repeat: SetTextureWrap(*texture_, TEXTURE_WRAP_REPEAT); break;
	case mirror_clamp: SetTextureWrap(*texture_, TEXTURE_WRAP_MIRROR_CLAMP); break;
	case mirror_repeat: SetTextureWrap(*texture_, TEXTURE_WRAP_MIRROR_REPEAT); break;
	}
}

void Sprite::set_texture_source_rect(rect<int> val, bool update_bounds) {
	texture_source_rect_ = val;

	if (update_bounds) {
		set_bounds({texture_source_rect_.width, texture_source_rect_.height});
	}
}

rect<int> Sprite::texture_source_rect() const {
	return texture_source_rect_;
}

// protected
bool Sprite::init() {
	if (!set_texture(file_arg_)) {
		errorlog("Failed");
		return false;
	}

	set_texture_wrap(texture_wrap::clamp);
	toggle_antialiasing(true);
	enable_draw();

	return true;
}

// protected
void Sprite::draw(mat3 const& transform, rgba color) {
	if (texture_) {
		ctx_.renderer.draw_texture(*texture_, texture_source_rect_, transform, color);
	}
}

} // namespace ae