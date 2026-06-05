#pragma once
#include <data/byte_buffer.hh>
#include <filesystem>
#include <memory>
#include <string_view>
#include <unordered_map>

namespace aether {

class Aether;

class AudioRepository final {
	friend class Aether;

private:
	AudioRepository();

public:
	~AudioRepository();
	AudioRepository(AudioRepository const&)            = delete;
	AudioRepository(AudioRepository&&)                 = delete;
	AudioRepository& operator=(AudioRepository const&) = delete;
	AudioRepository& operator=(AudioRepository&&)      = delete;

	[[nodiscard]] std::shared_ptr<byte_buffer> fetch(std::string_view file);
	void purge_unused();

private:
	void clear_cache();
	[[nodiscard]] std::shared_ptr<byte_buffer> try_fetch_from_cache(std::filesystem::path const& file) const;

	std::unordered_map<std::filesystem::path, std::shared_ptr<byte_buffer>> cache_;
};

} // namespace aether