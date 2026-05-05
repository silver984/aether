#include <aether/graphics/Texture.hh>
#include <raylib.h>

namespace ae {

Texture::Texture() :
	id_(0),
	mipmaps_(0),
	format_(0)
{}

Texture::~Texture() {
	if (id_ == 0) {
		return;
	}

	using raylib_texture = Texture2D;
	UnloadTexture(raylib_texture{.id = id_});
}

// private
Texture::Texture(
	std::string_view file,
	uint32_t id,
	size<int> const& bounds,
	int mipmaps,
	int format
) :
	file_(std::string(file)),
	id_(id),
	bounds_(bounds),
	mipmaps_(mipmaps),
	format_(format)
{}

std::shared_ptr<Texture> Texture::load_shared(char const* file) {
	using raylib_texture = Texture2D;
	raylib_texture tex = LoadTexture(file);

	if (tex.id == 0) {
		return nullptr;
	}

	return std::make_shared<Texture>(
		file,
		tex.id,
		size<int>(tex.width, tex.height),
		tex.mipmaps,
		tex.format
	);
}

std::string_view Texture::file() const {
	return file_;
}

uint32_t Texture::id() const {
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