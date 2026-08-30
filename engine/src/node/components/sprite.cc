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
	return true;
}

void sprite::set_texture_wrap(texture_wrap wrap_type) {
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

void sprite::set_texture_source_rect(rect<float> const& val, bool update_bounds) {
	texture_source_rect_ = val;
	if (update_bounds) {
		int w = (uint32_t)std::abs(std::round(texture_source_rect_.width));
		int h = (uint32_t)std::abs(std::round(texture_source_rect_.height));
		this->strong_node_()->component<transform>()->set_bounds(size<uint32_t>(w, h));
	}
}

bool sprite::init_() {
	if (!node_component::init_()) {
		return false;
	}
	use_fallback_texture_();
	return true;
}

void sprite::draw_() {
	node_component::draw_();
	strong_ref<node> n = this->strong_node_();
	visibility* v      = n->component<visibility>();

	if (v && !v->is_visible()) {
		return;
	}

	transform* t = n->component<transform>();
	this->ctx_.renderer->draw_texture(*texture_, texture_source_rect_, t->matrix(), rgba(255) /* temporary rgba */);
}

void sprite::use_fallback_texture_() {
	// assuming that the fallback texture always exists
	set_texture(zip_archive("aether.pak"), "null");
	set_texture_source_rect(rect<float>(0.f, 0.f, 100.f, 100.f));
	set_texture_wrap(texture_wrap::repeat);
}

} // namespace aether