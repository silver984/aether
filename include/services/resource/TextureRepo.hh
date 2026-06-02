#pragma once
#include <memory>
#include <unordered_set>
#include <util/path_map.hh>

struct Texture;

namespace aether {

class Aether;

class TextureRepo final {
	friend class Aether;

private:
	TextureRepo();

public:
	~TextureRepo();
	TextureRepo(TextureRepo const&)            = delete;
	TextureRepo(TextureRepo&&)                 = delete;
	TextureRepo& operator=(TextureRepo const&) = delete;
	TextureRepo& operator=(TextureRepo&&)      = delete;

	std::shared_ptr<Texture> fetch(std::string_view file);
	void purge_unused();

private:
	void clear();
	[[nodiscard]] std::shared_ptr<Texture> try_fetch_from_cache(std::filesystem::path const& file);
	[[nodiscard]] bool is_texture_valid(Texture const& texture);

	util::path_map<std::shared_ptr<Texture>> cache_;
};

} // namespace ae