#include <data/texture2d.hh>
#include <texture_cache.hh>

namespace aether {

texture_cache::texture_cache() noexcept
        : cache({".png", ".jpg", ".jpeg", ".bmp", ".gif", ".dds"}) {
}
texture_cache::~texture_cache() noexcept = default;

ref<texture2d> texture_cache::load_(fs::path const& file) {
	ref<texture2d> texture         = new texture2d(file.string());
	size<int> const texture_bounds = texture->bounds();
	bool const is_texture_valid    = texture->id() > 0 && texture_bounds.width > 0 && texture_bounds.height > 0;
	if (!is_texture_valid) {
		AETHER_ERRORLOG("Invalid texture properties");
		return nullptr;
	}
	return texture;
}

} // namespace aether