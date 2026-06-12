#include <debug/log.hh>
#include <texture2d.hh>
#include <texture_repository.hh>
#include <util/filesystem.hh>
#include <util/string.hh>
#include <util/timer.hh>
#include <utility>

namespace aether {

texture_repository::texture_repository()  = default;
texture_repository::~texture_repository() = default;

sref<texture2d> texture_repository::fetch(std::string_view file) {
	std::filesystem::path lfile = std::filesystem::weakly_canonical(file);

	if (!std::filesystem::exists(lfile)) {
		AETHER_ERRORLOG("File doesn't exist | file: \"{}\"", file);
		return nullptr;
	}

	if (auto from_cache = try_fetch_from_cache_(lfile)) {
		return from_cache;
	}

	if (std::string const file_extension = util::file_extension(lfile);
	    !util::string_matches_any(file_extension, {".png", ".jpg", ".jpeg", ".bmp", ".gif", ".dds"})) {
		AETHER_ERRORLOG("Unsupported file format | file: \"{}\"", file);
		return nullptr;
	}

	purge_unused();

	AETHER_DEBUGLOG("Loading \"{}\"", file);
	auto const start_time          = util::start();
	sref<texture2d> shared_texture = new texture2d(lfile.string());

	if (!is_texture_valid_(shared_texture)) {
		AETHER_ERRORLOG("Invalid texture properties");
	}

	auto texture_bounds = shared_texture->bounds();
	AETHER_TRACELOG("Allocated shared texture | bounds: {}x{} | id: {} | address: {}", texture_bounds.width, texture_bounds.height,
	                shared_texture->id(), fmt::ptr(shared_texture.get()));

	auto const [iterator, _] = cache_.emplace(lfile, std::move(shared_texture));
	auto const end_time      = util::end(start_time);
	AETHER_TRACELOG("Successfully inserted to cache | cache size: {}", cache_.size());
	AETHER_DEBUGLOG("Done | took {}ms", end_time);

	return iterator->second;
}

void texture_repository::purge_unused() {
	std::erase_if(cache_, [](auto const& pair) {
		return pair.second.strong_count() <= 1;
	});
}

void texture_repository::clear_cache_() {
	cache_.clear();
}

sref<texture2d> texture_repository::try_fetch_from_cache_(std::filesystem::path const& file) const {
	if (auto const iterator = cache_.find(file); iterator != cache_.end()) {
		return iterator->second;
	}

	return nullptr;
}

bool texture_repository::is_texture_valid_(sref<texture2d> texture) const {
	auto texture_bounds = texture->bounds();
	return texture->id() > 0 && texture_bounds.width > 0 && texture_bounds.height > 0;
}

} // namespace aether