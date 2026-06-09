#include <audio_repository.hh>
#include <debug/log.hh>
#include <util/filesystem.hh>
#include <util/string.hh>
#include <utility>

namespace aether {

audio_repository::audio_repository()  = default;
audio_repository::~audio_repository() = default;

std::shared_ptr<byte_buffer> audio_repository::fetch(std::string_view file) {
	std::filesystem::path lfile = std::filesystem::weakly_canonical(file);

	if (!std::filesystem::exists(lfile)) {
		AETHER_ERRORLOG("File doesn't exist | file: \"{}\"", file);
		return nullptr;
	}

	if (auto from_cache = try_fetch_from_cache_(lfile)) {
		return from_cache;
	}

	if (std::string const file_extension = util::file_extension(lfile);
	    !util::string_matches_any(file_extension, {".wav", ".mp3", ".ogg", ".flac"})) {
		AETHER_ERRORLOG("Unsupported file format | file: \"{}\"", file);
		return nullptr;
	}

	purge_unused();

	byte_buffer temporary_buffer = util::read_file_to_byte_buffer(lfile);

	if (temporary_buffer.empty()) {
		AETHER_ERRORLOG("Failed | empty buffer");
		return nullptr;
	}

	auto shared_buffer       = std::make_shared<byte_buffer>(std::move(temporary_buffer));
	auto const [iterator, _] = cache_.emplace(lfile, std::move(shared_buffer));
	return iterator->second;
}

void audio_repository::purge_unused() {
	std::erase_if(cache_, [](auto const& pair) {
		return pair.second.use_count() <= 1;
	});
}

void audio_repository::clear_cache_() {
	cache_.clear();
}

std::shared_ptr<byte_buffer> audio_repository::try_fetch_from_cache_(std::filesystem::path const& file) const {
	if (auto const iterator = cache_.find(file); iterator != cache_.end()) {
		return iterator->second;
	}

	return nullptr;
}

} // namespace aether