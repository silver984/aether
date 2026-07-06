#pragma once
#include <filesystem>
#include <string_view>
#include <unordered_map>
#include <util/ref.hh>

namespace aether {

class game;
class texture2d;

class texture_repository final {
	friend class game;

public:
	~texture_repository();
	[[nodiscard]] ref<texture2d> fetch(std::string_view file);
	void purge_unused();

private:
	texture_repository();
	void clear_cache_();
	[[nodiscard]] ref<texture2d> try_fetch_from_cache_(std::filesystem::path const& file) const;
	[[nodiscard]] bool is_texture_valid_(ref<texture2d> texture) const;

	std::unordered_map<std::filesystem::path, ref<texture2d>> cache_;
};

} // namespace aether