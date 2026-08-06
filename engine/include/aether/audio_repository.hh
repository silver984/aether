#pragma once
#include <aether/blob.hh>
#include <aether/ref.hh>
#include <filesystem>
#include <string_view>
#include <unordered_map>

namespace aether {

class game;

class audio_repository final {
	friend class game;

public:
	~audio_repository();
	[[nodiscard]] strong_ref<blob> fetch(std::string_view file);
	void purge_unused();

private:
	audio_repository();
	void clear_cache_();
	[[nodiscard]] strong_ref<blob> try_fetch_from_cache_(std::filesystem::path const& file) const;

	std::unordered_map<std::filesystem::path, strong_ref<blob>> cache_;
};

} // namespace aether