#include <audio_repository.hh>
#include <debug/log.hh>
#include <util/filesystem.hh>
#include <util/string.hh>
#include <utility>

namespace aether {

audio_repository::audio_repository()  = default;
audio_repository::~audio_repository() = default;

strong_ref<blob> audio_repository::fetch(std::string_view file) {
	std::filesystem::path lfile = std::filesystem::weakly_canonical(file);

	if (!std::filesystem::exists(lfile)) {
		AETHER_ERRORLOG("File doesn't exist ? file: \"{}\"", file);
		return nullptr;
	}

	if (auto from_cache = try_fetch_from_cache_(lfile)) {
		return from_cache;
	}

	if (std::string const file_extension = util::file_extension(lfile);
	    !util::string_matches_any(file_extension, {".wav", ".mp3", ".ogg", ".flac"})) {
		AETHER_ERRORLOG("Unsupported file format ? file: \"{}\"", file);
		return nullptr;
	}

	purge_unused();

	blob temporary_buffer = util::read_file_to_buffer(lfile);

	if (temporary_buffer.empty()) {
		AETHER_ERRORLOG("Failed ? empty buffer");
		return nullptr;
	}

	strong_ref<blob> shared_buffer = new blob(std::move(temporary_buffer));
	auto const [it, _]             = cache_.emplace(lfile, std::move(shared_buffer));
	return it->second;
}

void audio_repository::purge_unused() {
	std::erase_if(cache_, [](auto const& pair) {
		return pair.second.strong_count() <= 1;
	});
}

void audio_repository::clear_cache_() {
	cache_.clear();
}

strong_ref<blob> audio_repository::try_fetch_from_cache_(std::filesystem::path const& file) const {
	if (auto const it = cache_.find(file); it != cache_.end()) {
		return it->second;
	}

	return nullptr;
}

} // namespace aether