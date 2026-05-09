#include <aether/objects/Graphic.hh>
#include <aether/systems/Resource.hh>
#include <aether/systems/Renderer.hh>
#include <aether/common/rl_adapter.hh>
#include <raylib.h>

namespace ae {

Graphic::Graphic(std::string_view file) :
	file_arg_(std::string(file))
{}

Graphic::~Graphic() = default;

std::string_view Graphic::type() const {
	return "Graphic";
}

void Graphic::toggle_antialiasing(bool val) const {
	if (texture_) {
		SetTextureFilter(*texture_, val ? TextureFilter::TEXTURE_FILTER_BILINEAR : TextureFilter::TEXTURE_FILTER_POINT);
	}
}

// protected
bool Graphic::init(Context const& ctx) {
	auto resource = ctx.resource().lock();

	if (!resource) {
		// TODO: log error
		return false;
	}

	if (texture_ = resource->load_shared_texture(file_arg_)) {
		toggle_antialiasing(true);
		
		size<float> bounds = {
			.width = static_cast<float>(texture_->width),
			.height = static_cast<float>(texture_->height)
		};

		set_bounds(bounds);

		return true;
	}

	// TODO: log error
	return false;
}

// protected
void Graphic::draw(Context const& ctx, mat3 const& transform, float alpha) const {
	auto renderer = ctx.renderer().lock();

	if (!renderer || !texture_) {
		return;
	}

	renderer->draw_texture(*texture_, transform, color(), alpha);
}

}