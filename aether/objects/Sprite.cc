#include <aether/objects/Sprite.hh>
#include <aether/systems/Resource.hh>
#include <aether/systems/Renderer.hh>
#include <aether/common/log.hh>
#include <raylib.h>
#include <rlgl.h>

namespace ae {

Sprite::Sprite(std::string_view file) :
	file_arg_(std::string(file))
{}

Sprite::~Sprite() = default;

std::string_view Sprite::type() const {
	return "Sprite";
}

void Sprite::toggle_antialiasing(bool val) const {
	if (texture_) {
		SetTextureFilter(*texture_, val ? TextureFilter::TEXTURE_FILTER_BILINEAR : TextureFilter::TEXTURE_FILTER_POINT);
	}
}

bool Sprite::set_texture(std::string_view file) {
	auto ctx = context();

	if (!ctx) {
		errorlog("Can't reference engine context");
		return false;
	}

	auto resource = ctx->resource_wref().lock();

	if (!resource) {
		errorlog("Can't reference resource system");
		return false;
	}

	texture_ = resource->load_shared_texture(file);

	if (!texture_) {
		errorlog("Failed");
		return false;
	}

	size<float> new_bounds = {
		.width = static_cast<float>(texture_->width),
		.height = static_cast<float>(texture_->height)
	};

	set_bounds(new_bounds);

	texture_source_rect_ = {
		.x = 0.f,
		.y = 0.f,
		.width = static_cast<float>(texture_->width),
		.height = static_cast<float>(texture_->height)
	};

	return true;
}

void Sprite::set_texture_wrap(texture_wrap type) {
	if (!texture_) {
		debuglog("Attempted to set texture wrap with nullptr texture");
		return;
	}

	switch (type) {
		using enum texture_wrap;
	case clamp:
		SetTextureWrap(*texture_, TextureWrap::TEXTURE_WRAP_CLAMP);
		break;
	case repeat:
		SetTextureWrap(*texture_, TextureWrap::TEXTURE_WRAP_REPEAT);
		break;
	case mirror_clamp:
		SetTextureWrap(*texture_, TextureWrap::TEXTURE_WRAP_MIRROR_CLAMP);
		break;
	case mirror_repeat:
		SetTextureWrap(*texture_, TextureWrap::TEXTURE_WRAP_MIRROR_REPEAT);
		break;
	}
}

void Sprite::set_texture_source_rect(rect<float> val, bool update_bounds) {
	texture_source_rect_ = val;
	
	if (update_bounds) {
		set_bounds(val.bounds<float>());
	}
}

rect<float> Sprite::texture_source_rect() const {
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

	return true;
}

// protected
void Sprite::draw(mat3 const& transform, rgb color, float alpha) const {
	auto ctx = context();

	if (!ctx) {
		return;
	}

	auto renderer = ctx->renderer_wref().lock();

	if (!renderer || !texture_) {
		return;
	}

	renderer->draw_texture(*texture_, texture_source_rect_, transform, color, alpha);
}

}