#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <raylib.h>
#include <services/resource/texture_repository.hh>
#include <util/filesystem.hh>
#include <util/string.hh>
#include <util/timer.hh>
#include <utility>

namespace {

struct texture_deleter {
	void operator()(rltexture* ptr) const {
		if (ptr && ptr->id > 0) {
			UnloadTexture(*ptr);
		}

		delete ptr;
	}
};

} // namespace

namespace aether {

// private
texture_repository::texture_repository()  = default;
texture_repository::~texture_repository() = default;

std::shared_ptr<rltexture> texture_repository::fetch(std::string_view file) {
	std::filesystem::path lfile = std::filesystem::weakly_canonical(file);

	if (!std::filesystem::exists(lfile)) {
#ifdef AETHER_DEBUG
		errorlog("File doesn't exist | file: \"{}\"", file);
#endif
		return nullptr;
	}

	if (std::shared_ptr<rltexture> from_cache = try_fetch_from_cache(lfile)) {
		return from_cache;
	}

	if (std::string const file_extension = util::file_extension(lfile);
	    !util::string_matches_any(file_extension, {".png", ".jpg", ".jpeg", ".bmp", ".gif", ".dds"})) {
#ifdef AETHER_DEBUG
		errorlog("Unsupported file format | file: \"{}\"", file);
#endif
		return nullptr;
	}

	purge_unused();

#ifdef AETHER_VERBOSE_DEBUG
	debuglog("Loading \"{}\"", file);
	auto const start_time = util::start();
#endif

	rltexture temporary_texture = LoadTexture(lfile.string().c_str());

	if (!is_texture_valid(temporary_texture)) {
#ifdef AETHER_DEBUG
		errorlog("Invalid texture properties");
#endif
		UnloadTexture(temporary_texture);
	}

	std::shared_ptr<rltexture> shared_texture =
	    std::shared_ptr<rltexture>(new rltexture(std::move(temporary_texture)), texture_deleter{});

#ifdef AETHER_VERBOSE_DEBUG
	tracelog("Allocated shared texture | bounds: {}x{} | id: {} | address: {}", shared_texture->width,
	         shared_texture->height, shared_texture->id, fmt::ptr(shared_texture.get()));
#endif

	auto const [iterator, _] = cache_.emplace(lfile, std::move(shared_texture));

#ifdef AETHER_VERBOSE_DEBUG
	auto const end_time = util::end(start_time);
	tracelog("Successfully inserted to cache | cache size: {}", cache_.size());
	debuglog("Done | took {}ms", end_time);
#endif

	return iterator->second;
}

void texture_repository::purge_unused() {
	std::erase_if(cache_, [](auto const& pair) {
		return pair.second.use_count() <= 1;
	});
}

// private
void texture_repository::clear_cache() {
	cache_.clear();
}

// private
std::shared_ptr<rltexture> texture_repository::try_fetch_from_cache(std::filesystem::path const& file) const {
	if (auto const iterator = cache_.find(file); iterator != cache_.end()) {
		return iterator->second;
	}

	return nullptr;
}

// private
bool texture_repository::is_texture_valid(rltexture const& texture) const {
	return texture.id > 0 && texture.width > 0 && texture.height > 0;
}

} // namespace aether