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

public:
	~texture_repository();
	texture_repository(texture_repository const&)            = delete;
	texture_repository(texture_repository&&)                 = delete;
	texture_repository& operator=(texture_repository const&) = delete;
	texture_repository& operator=(texture_repository&&)      = delete;
	[[nodiscard]] std::shared_ptr<rltexture> fetch(std::string_view file);
	void purge_unused();

private:
	texture_repository();
	void clear_cache_();
	[[nodiscard]] std::shared_ptr<rltexture> try_fetch_from_cache_(std::filesystem::path const& file) const;
	[[nodiscard]] bool is_texture_valid_(rltexture const& texture) const;

	std::unordered_map<std::filesystem::path, std::shared_ptr<rltexture>> cache_;
};

} // namespace aether