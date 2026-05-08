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

std::optional<std::string_view> Graphic::file_name() const {
	if (texture_) {
		return texture_->file();
	}

	return std::nullopt;
}

void Graphic::toggle_antialiasing(bool val) const {
	if (texture_) {
		SetTextureFilter(rl::to_Texture2D(*texture_), val ? TextureFilter::TEXTURE_FILTER_BILINEAR : TextureFilter::TEXTURE_FILTER_POINT);
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
		auto texture_bounds = texture_->bounds();
		set_bounds(size<float>((float)texture_bounds.width, (float)texture_bounds.height));
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