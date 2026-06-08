#pragma once
#include <filesystem>
#include <memory>
#include <string_view>
#include <unordered_map>

struct Texture;
using rltexture = Texture;

namespace aether {

class game;

class texture_repository final {
	friend class game;

    private:
	texture_repository();

    public:
	~texture_repository();
	texture_repository(texture_repository const&)            = delete;
	texture_repository(texture_repository&&)                 = delete;
	texture_repository& operator=(texture_repository const&) = delete;
	texture_repository& operator=(texture_repository&&)      = delete;

	[[nodiscard]] std::shared_ptr<rltexture> fetch(std::string_view file);
	void purge_unused();

    private:
	void clear_cache();
	[[nodiscard]] std::shared_ptr<rltexture> try_fetch_from_cache(std::filesystem::path const& file) const;
	[[nodiscard]] bool is_texture_valid(rltexture const& texture) const;

	std::unordered_map<std::filesystem::path, std::shared_ptr<rltexture>> cache_;
};

} // namespace aether