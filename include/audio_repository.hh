#pragma once
#include <byte_buffer.hh>
#include <filesystem>
#include <memory>
#include <string_view>
#include <unordered_map>

namespace aether {

class game;

class audio_repository final {
	friend class game;

    private:
	audio_repository();

    public:
	~audio_repository();
	audio_repository(audio_repository const&)            = delete;
	audio_repository(audio_repository&&)                 = delete;
	audio_repository& operator=(audio_repository const&) = delete;
	audio_repository& operator=(audio_repository&&)      = delete;

	[[nodiscard]] std::shared_ptr<byte_buffer> fetch(std::string_view file);
	void purge_unused();

    private:
	void clear_cache();
	[[nodiscard]] std::shared_ptr<byte_buffer> try_fetch_from_cache(std::filesystem::path const& file) const;

	std::unordered_map<std::filesystem::path, std::shared_ptr<byte_buffer>> cache_;
};

} // namespace aether