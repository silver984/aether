#pragma once
#include <filesystem>
#include <string_view>
#include <unordered_map>
#include <util/blob.hh>
#include <util/ref.hh>


namespace aether {

class game;

class audio_repository final {
	friend class game;

public:
	~audio_repository();
	[[nodiscard]] ref<blob> fetch(std::string_view file);
	void purge_unused();

private:
	audio_repository();
	void clear_cache_();
	[[nodiscard]] ref<blob> try_fetch_from_cache_(std::filesystem::path const& file) const;

	std::unordered_map<std::filesystem::path, ref<blob>> cache_;
};

} // namespace aether