#include <cmath>
#include <context.hh>
#include <data/texture2d.hh>
#include <debug/log.hh>
#include <nodes/sprite.hh>
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
	SetTextureFilter(texture_->get(), val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
}

bool sprite::set_texture(std::string_view file) {
	if (auto fetched_texture = ctx_().textures().fetch(file)) {
		texture_ = fetched_texture;
	} else {
		AETHER_ERRORLOG("Failed");
		return false;
	}

	size<int> const new_bounds = texture_->bounds();
	texture_source_rect_       = rect<float>(0.f, 0.f, (float)new_bounds.width, (float)new_bounds.height);
	set_bounds(new_bounds);

	return true;
}

void sprite::set_texture_wrap(texture_wrap type) {
	switch (type) {
		using enum texture_wrap;
	case CLAMP: SetTextureWrap(texture_->get(), TEXTURE_WRAP_CLAMP); break;
	case REPEAT: SetTextureWrap(texture_->get(), TEXTURE_WRAP_REPEAT); break;
	case MIRROR_CLAMP: SetTextureWrap(texture_->get(), TEXTURE_WRAP_MIRROR_CLAMP); break;
	case MIRROR_REPEAT: SetTextureWrap(texture_->get(), TEXTURE_WRAP_MIRROR_REPEAT); break;
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
	if (!set_texture(file_arg_)) {
		AETHER_ERRORLOG("Failed");
		return false;
	}

	set_texture_wrap(wrap_type_arg_);
	toggle_antialiasing(has_antialiasing_arg_);
	schedule_draw();

	return true;
}

void sprite::draw_(mat3 const& transform, rgba color) {
	ctx_().get_renderer().draw_texture(texture_->get(), texture_source_rect_, transform, color);
}

} // namespace aether