#pragma once
#include <filesystem>
#include <sref.hh>
#include <string_view>
#include <unordered_map>

namespace aether {

class game;
class texture2d;

class texture_repository final {
	friend class game;

public:
	~texture_repository();
	[[nodiscard]] sref<texture2d> fetch(std::string_view file);
	void purge_unused();

private:
	texture_repository();
	void clear_cache_();
	[[nodiscard]] sref<texture2d> try_fetch_from_cache_(std::filesystem::path const& file) const;
	[[nodiscard]] bool is_texture_valid_(sref<texture2d> texture) const;

	std::unordered_map<std::filesystem::path, sref<texture2d>> cache_;
};

} // namespace aether