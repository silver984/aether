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
	uint32_t id,
	size<int> const& bounds,
	int mipmaps,
	int format
) :
	id_(id),
	bounds_(bounds),
	mipmaps_(mipmaps),
	format_(format)
{}

sptr<Texture> Texture::load_shared(char const* file_path) {
	using raylib_texture = Texture2D;
	raylib_texture tex = LoadTexture(file_path);

	if (tex.id == 0) {
		return nullptr;
	}

	return shared<Texture>(Texture(
		tex.id,
		size<int>(tex.width, tex.height),
		tex.mipmaps,
		tex.format
	));
}

}