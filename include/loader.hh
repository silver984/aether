#pragma once
#include <util/blob.hh>
#include <util/ref.hh>

struct Texture;

namespace aether {

template <typename>
struct loader;

template <>
struct loader<Texture> final {
	static ref<Texture> load(blob& buffer) noexcept;
	static void unload(ref<Texture>& texture) noexcept;
};

} // namespace aether