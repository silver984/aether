#include <aether/loader.hh>
#include <raylib.h>

namespace aether {

strong_ref<Texture> loader<Texture>::load(std::string_view filename, blob& buffer) {
	Image img = LoadImageFromMemory(".png", reinterpret_cast<unsigned char*>(buffer.data()), (int)buffer.size());

	if (!IsImageValid(img)) {
		return nullptr;
	}

	auto tex = strong_ref<Texture>::make(LoadTextureFromImage(img));

	if (!IsTextureValid(*tex)) {
		return nullptr;
	}

	UnloadImage(img);
	return tex;
};

void loader<Texture>::unload(Texture& texture) {
	UnloadTexture(texture);
}

} // namespace aether