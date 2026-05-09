#include <aether/objects/Sprite.hh>
#include <aether/systems/Resource.hh>
#include <aether/systems/Renderer.hh>
#include <aether/common/log.hh>
#include <raylib.h>

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

// protected
bool Sprite::init(Context const& ctx) {
	auto resource = ctx.resource().lock();

	if (!resource) {
		errorlog("Can't reference resource system");
		return false;
	}

	texture_ = resource->load_shared_texture(file_arg_);

	toggle_antialiasing(true);
		
	size<float> bounds = {
		.width = static_cast<float>(texture_->width),
		.height = static_cast<float>(texture_->height)
	};

	set_bounds(bounds);

	return true;
}

// protected
void Sprite::draw(Context const& ctx, mat3 const& transform, rgb color, float alpha) const {
	auto renderer = ctx.renderer().lock();

	if (!renderer || !texture_) {
		return;
	}

	renderer->draw_texture(*texture_, transform, color, alpha);
}

}