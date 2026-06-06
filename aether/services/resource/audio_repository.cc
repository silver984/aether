#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <services/resource/audio_repository.hh>
#include <util/filesystem.hh>
#include <util/string.hh>
#include <utility>

namespace aether {

// private
audio_repository::audio_repository()  = default;
audio_repository::~audio_repository() = default;

std::shared_ptr<byte_buffer> audio_repository::fetch(std::string_view file) {
	std::filesystem::path lfile = std::filesystem::weakly_canonical(file);

	if (!std::filesystem::exists(lfile)) {
#ifdef AETHER_DEBUG
		errorlog("File doesn't exist | file: \"{}\"", file);
#endif
		return nullptr;
	}

	if (std::shared_ptr<byte_buffer> from_cache = try_fetch_from_cache(lfile)) {
		return from_cache;
	}

	if (std::string const file_extension = util::file_extension(lfile);
	    !util::string_matches_any(file_extension, {".wav", ".mp3", ".ogg", ".flac"})) {
#ifdef AETHER_DEBUG
		errorlog("Unsupported file format | file: \"{}\"", file);
#endif
		return nullptr;
	}

	purge_unused();

	byte_buffer temporary_buffer = util::read_file_to_byte_buffer(lfile);

	if (temporary_buffer.empty()) {
#ifdef AETHER_DEBUG
		errorlog("Failed | empty buffer");
#endif
		return nullptr;
	}

	std::shared_ptr<byte_buffer> shared_buffer = std::make_shared<byte_buffer>(std::move(temporary_buffer));
	auto const [iterator, _]                   = cache_.emplace(lfile, std::move(shared_buffer));
	return iterator->second;
}

void audio_repository::purge_unused() {
	std::erase_if(cache_, [](auto const& pair) {
		return pair.second.use_count() <= 1;
	});
}

// private
void audio_repository::clear_cache() {
	cache_.clear();
}

// private
std::shared_ptr<byte_buffer> audio_repository::try_fetch_from_cache(std::filesystem::path const& file) const {
	if (auto const iterator = cache_.find(file); iterator != cache_.end()) {
		return iterator->second;
	}

	return nullptr;
}

} // namespace aether