#pragma once
#include <data/u8vec.hh>
#include <filesystem>
#include <sref.hh>
#include <string_view>
#include <unordered_map>

namespace aether {

class game;

class audio_repository final {
	friend class game;

public:
	~audio_repository();
	[[nodiscard]] sref<u8vec> fetch(std::string_view file);
	void purge_unused();

private:
	audio_repository();
	void clear_cache_();
	[[nodiscard]] sref<u8vec> try_fetch_from_cache_(std::filesystem::path const& file) const;

	std::unordered_map<std::filesystem::path, sref<u8vec>> cache_;
};

} // namespace aether