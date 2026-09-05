#include <aether/context.hh>
#include <aether/node/components/sprite.hh>
#include <aether/node/components/transform.hh>
#include <aether/node/components/visibility.hh>
#include <aether/node/node.hh>
#include <aether/renderer.hh>
#include <aether/resources.hh>
#include <raylib.h>

namespace aether {

void sprite::set_antialiasing(bool val) {
	if (!texture_) {
		return;
	}
	int filter = val ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT;
	SetTextureFilter(*texture_, filter);
}

bool sprite::set_texture(zip_archive const& pak, std::string_view file) {
	if (strong_ref<Texture> tx = this->ctx_.textures->load(pak, file)) {
		texture_ = tx;
	} else {
		return false;
	}
	float w = (float)texture_->width;
	float h = (float)texture_->height;
	set_texture_source_rect(rect<float>(0.f, 0.f, w, h));
	update_transform_bounds();
	return true;
}

void sprite::set_texture_wrap(texture_wrap wrap_type) {
	if (!texture_) {
		return;
	}

	switch (wrap_type) {
	case texture_wrap::clamp: {
		SetTextureWrap(*texture_, TEXTURE_WRAP_CLAMP);
		break;
	}
	case texture_wrap::repeat: {
		SetTextureWrap(*texture_, TEXTURE_WRAP_REPEAT);
		break;
	}
	case texture_wrap::mirror_clamp: {
		SetTextureWrap(*texture_, TEXTURE_WRAP_MIRROR_CLAMP);
		break;
	}
	case texture_wrap::mirror_repeat: {
		[[fallthrough]];
	}
	default: {
		SetTextureWrap(*texture_, TEXTURE_WRAP_MIRROR_REPEAT);
		break;
	}
	}
}

bool sprite::update_transform_bounds() {
	transform* t = this->strong_node_()->component<transform>();
	if (!t) {
		return false;
	}
	uint32_t w = (uint32_t)std::abs(std::round(texture_source_rect_.width));
	uint32_t h = (uint32_t)std::abs(std::round(texture_source_rect_.height));
	t->set_bounds(size<uint32_t>(w, h));
	return true;
}

void sprite::draw_() {
	node_component::draw_();

	if (!texture_) {
		return;
	}

	strong_ref<node> n = this->strong_node_();
	visibility* v      = n->component<visibility>();

	if (v && !v->is_visible()) {
		return;
	}

	transform* t = n->component<transform>();
	this->ctx_.renderer->draw_texture(*texture_, texture_source_rect_, t ? t->matrix() : mat3::identity(),
	                                  rgba(255) /* temporary rgba */);
}

} // namespace aether