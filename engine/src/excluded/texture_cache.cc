#include <data/texture2d.hh>
#include <texture_cache.hh>

namespace aether {

texture_cache::texture_cache()  = default;
texture_cache::~texture_cache() = default;

strong_ref<texture2d> texture_cache::load_(fs::path const& file) {
	strong_ref<texture2d> texture  = new texture2d(file.string());
	size<int> const texture_bounds = texture->bounds();
	bool const is_texture_valid    = texture->id() > 0 && texture_bounds.width > 0 && texture_bounds.height > 0;
	if (!is_texture_valid) {
		ae_error("Invalid texture properties");
		return nullptr;
	}
	return texture;
}

} // namespace aether