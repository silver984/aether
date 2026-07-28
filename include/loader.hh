#pragma once
#include <util/blob.hh>
#include <util/ref.hh>

struct Texture;

namespace aether {

template <typename>
struct loader;

template <>
struct loader<Texture> final {
	loader() = delete;
	static strong_ref<Texture> load(blob& buffer);
	static void unload(Texture& texture);
};

} // namespace aether