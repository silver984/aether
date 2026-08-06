#pragma once
#include <aether/blob.hh>
#include <aether/ref.hh>
#include <string_view>

struct Texture;

namespace aether {

template <typename>
struct loader;

template <>
struct loader<Texture> final {
	loader() = delete;
	[[nodiscard]] static strong_ref<Texture> load(std::string_view filename, blob& buffer);
	static void unload(Texture& texture);
};

} // namespace aether