#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <Context.hh>
#include <raylib.h>
#include <services/resource/TextureRepository.hh>
#include <util/filesystem.hh>
#include <util/string.hh>
#include <util/timer.hh>
#include <utility>

namespace {

struct texture_deleter {
	void operator()(Texture* ptr) const {
		if (ptr && ptr->id > 0) {
			UnloadTexture(*ptr);
		}

		delete ptr;
	}
};

} // namespace

namespace aether {

// private
TextureRepository::TextureRepository()  = default;
TextureRepository::~TextureRepository() = default;

std::shared_ptr<Texture> TextureRepository::fetch(std::string_view file) {
	std::filesystem::path lfile;

	if (auto const optional_file = util::normalized_filepath(file); optional_file.has_value()) {
		lfile = optional_file.value();
	} else {
#ifdef AETHER_VERBOSE_DEBUG
		errorlog("Filesystem gave an error");
#endif
		return nullptr;
	}

	if (auto from_cache = try_fetch_from_cache(lfile)) {
		return from_cache;
	}

	if (auto const file_extension = util::file_extension(lfile);
	    !util::string_matches_any(file_extension, {".png", ".jpg", ".jpeg", ".bmp", ".gif", ".dds"})) {
#ifdef AETHER_DEBUG
		errorlog("Unsupported file format | file: \"{}\"", lfile.filename().string());
#endif
		return nullptr;
	}

	purge_unused();

#ifdef AETHER_VERBOSE_DEBUG
	debuglog("Loading \"{}\"", lfile.filename().string());
	auto const start_time = util::start();
#endif

	Texture temporary_texture = LoadTexture(lfile.string().c_str());

	if (!is_texture_valid(temporary_texture)) {
#ifdef AETHER_DEBUG
		errorlog("Invalid texture properties");
#endif
		UnloadTexture(temporary_texture);
	}

	std::shared_ptr<Texture> shared_texture =
	    std::shared_ptr<Texture>(new Texture(std::move(temporary_texture)), texture_deleter{});

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

void TextureRepository::purge_unused() {
	std::erase_if(cache_, [](auto& pair) {
		return pair.second.use_count() <= 1;
	});
}

// private
void TextureRepository::clear() {
	cache_.clear();
}

// private
std::shared_ptr<Texture> TextureRepository::try_fetch_from_cache(std::filesystem::path const& file) {
	if (auto const iterator = cache_.find(file); iterator != cache_.end()) {
		return iterator->second;
	}

	return nullptr;
}

// private
bool TextureRepository::is_texture_valid(Texture const& texture) {
	if (texture.id < 1) {
		return false;
	}

	if (texture.width < 1 || texture.height < 1) {
		return false;
	}

	return true;
}

} // namespace aether