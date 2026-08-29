#pragma once
#include <aether/ref.hh>
#include <string_view>

struct Texture;

namespace aether {

class zip_archive;

template <typename>
struct loader;

template <>
struct loader<Texture> final {
	loader() = delete;
	[[nodiscard]] static strong_ref<Texture> load(zip_archive& arc, std::string_view filename);
	static void unload(Texture& texture);
};

} // namespace aether