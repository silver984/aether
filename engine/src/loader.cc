#include <aether/blob.hh>
#include <aether/loader.hh>
#include <aether/zip_archive.hh>
#include <raylib.h>
#include <string>

namespace aether {

strong_ref<Texture> loader<Texture>::load(zip_archive const& arc, std::string_view filename) {
	std::string fn(filename);
	fn.append(".png");

	if (!arc.contains(fn)) {
		return nullptr;
	}

	blob buffer = arc.read(fn);

	if (buffer.empty()) {
		return nullptr;
	}

	Image img = LoadImageFromMemory(".png", reinterpret_cast<unsigned char*>(buffer.data()), (int)buffer.size());

	if (!IsImageValid(img)) {
		return nullptr;
	}

	auto tex = strong_ref<Texture>::create(LoadTextureFromImage(img));

	if (!IsTextureValid(*tex)) {
		return nullptr;
	}

	UnloadImage(img);
	return tex;
};

void loader<Texture>::unload(Texture const& texture) {
	UnloadTexture(texture);
}

} // namespace aether