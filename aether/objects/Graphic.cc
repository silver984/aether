#include <aether/objects/Graphic.hh>
#include <aether/systems/Resource.hh>
#include <aether/systems/Renderer.hh>

namespace ae {

Graphic::Graphic(std::string_view file) :
	file_arg_(std::string(file))
{}

Graphic::~Graphic() = default;

std::string_view Graphic::type() const {
	return "Graphic";
}

bool Graphic::init(Context const& ctx) {
	auto resource = ctx.resource();

	if (!resource) {
		// TODO: log error
		return false;
	}

	if (texture_ = resource->load_shared_texture(file_arg_)) {
		return true;
	}

	// TODO: log error
	return false;
}

void Graphic::draw(Context const& ctx, mat3 const& transform, float alpha) const {
	auto renderer = ctx.renderer();

	if (!renderer) {
		return;
	}

	renderer->draw_texture(*texture_, transform, alpha);
}

}