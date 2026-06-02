#pragma once
#include <memory>
#include <unordered_set>
#include <util/path_map.hh>

struct Texture;

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

	std::shared_ptr<Texture> fetch(std::string_view file);
	void purge_unused();

private:
	void clear();
	[[nodiscard]] std::shared_ptr<Texture> try_fetch_from_cache(std::filesystem::path const& file);
	[[nodiscard]] bool is_texture_valid(Texture const& texture);

	util::path_map<std::shared_ptr<Texture>> cache_;
};

} // namespace aether