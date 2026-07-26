#include <loader.hh>
#include <raylib.h>


namespace aether {

ref<Texture> loader<Texture>::load(blob& buffer) noexcept {
	Image img = LoadImageFromMemory(".png", reinterpret_cast<unsigned char*>(buffer.data()), (int)buffer.size());

	if (!IsImageValid(img)) {
		return nullptr;
	}

	ref<Texture> tex = new Texture(LoadTextureFromImage(img));
	UnloadImage(img);
	return tex;
};

void loader<Texture>::unload(ref<Texture>& texture) noexcept {
	UnloadTexture(*texture);
}

} // namespace aether