#pragma once
#include <filesystem>
#include <memory>
#include <string_view>
#include <unordered_map>

struct Texture;
using rltexture = Texture;

namespace aether {

class Aether;

class TextureRepository final {
	friend class Aether;

private:
	TextureRepository();

public:
	~TextureRepository();
	TextureRepository(TextureRepository const&)            = delete;
	TextureRepository(TextureRepository&&)                 = delete;
	TextureRepository& operator=(TextureRepository const&) = delete;
	TextureRepository& operator=(TextureRepository&&)      = delete;

	[[nodiscard]] std::shared_ptr<rltexture> fetch(std::string_view file);
	void purge_unused();

private:
	void clear_cache();
	[[nodiscard]] std::shared_ptr<rltexture> try_fetch_from_cache(std::filesystem::path const& file) const;
	[[nodiscard]] bool is_texture_valid(rltexture const& texture) const;

	std::unordered_map<std::filesystem::path, std::shared_ptr<rltexture>> cache_;
};

} // namespace aether