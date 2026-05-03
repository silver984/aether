#include <aether/graphics/texture.hh>
#include <raylib.h>

namespace ae {

texture::~texture() {
	if (id == 0) {
		return;
	}

	UnloadTexture({.id = id});
}

texture texture::load(char const* file_path) {
	Texture rl_texture = LoadTexture(file_path);

	return {
		.id = rl_texture.id,
		.bounds = size<int>(rl_texture.width, rl_texture.height),
		.mipmaps = rl_texture.mipmaps,
		.format = rl_texture.format
	};
}

}