#include <aether/graphics/Texture.hh>
#include <aether/common/log.hh>
#include <fmt/format.h>
#include <raylib.h>

namespace ae {

Texture::Texture() :
	id_(0),
	mipmaps_(0),
	format_(0)
{}

Texture::Texture(
	std::string_view file,
	unsigned int id,
	size<int> bounds,
	int mipmaps,
	int format
) :
	file_(std::string(file)),
	id_(id),
	bounds_(bounds),
	mipmaps_(mipmaps),
	format_(format)
{}

Texture::~Texture() {
	if (id_ == 0) {
		return;
	}

	UnloadTexture(Texture2D{.id = id_});

	log::trace(fmt::format("Unloaded ({}) | id: {}", fmt::ptr(this), id_));
}

std::shared_ptr<Texture> Texture::make_shared(char const* file) {
	Texture2D tex = LoadTexture(file);

	if (tex.id == 0) {
		return nullptr;
	}

	size<int> bounds = {
		.width = tex.width,
		.height = tex.height
	};

	auto ret = std::make_shared<Texture>(file, tex.id, bounds, tex.mipmaps, tex.format);

	log::trace(fmt::format("Made shared texture ({}) | id: {} | bounds: {}x{}",
		fmt::ptr(ret.get()), tex.id, bounds.width, bounds.height
	));

	return ret;
}

std::string_view Texture::file() const {
	return file_;
}

unsigned int Texture::id() const {
	return id_;
}

size<int> Texture::bounds() const {
	return bounds_;
}

int Texture::mipmaps() const {
	return mipmaps_;
}

int Texture::format() const {
	return format_;
}

}