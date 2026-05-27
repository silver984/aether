#include <aether/debug/log.hh>
#include <aether/systems/Context.hh>
#include <aether/systems/repos/TextureRepo.hh>
#include <aether/util/filesystem.hh>
#include <aether/util/string.hh>
#include <aether/util/timer.hh>
#include <raylib.h>
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

namespace ae {

// private
TextureRepo::TextureRepo()  = default;
TextureRepo::~TextureRepo() = default;

std::shared_ptr<Texture> TextureRepo::fetch(std::string_view file) {
	std::filesystem::path lfile;

	if (auto const optional_file = util::fs::normalized_filepath(file); optional_file.has_value()) {
		lfile = optional_file.value();
	} else {
		errorlog("Filesystem gave an error");
		return nullptr;
	}

	if (auto from_cache = try_fetch_from_cache(lfile)) {
		return from_cache;
	}

	if (auto const file_extension = util::fs::file_extension(lfile);
	    !util::str::string_matches_any(file_extension, {".png", ".jpg", ".jpeg", ".bmp", ".gif", ".dds"})) {
		errorlog("Unsupported file format | file: \"{}\"", lfile.filename().string());
		return nullptr;
	}

	purge_unused();

	debuglog("Loading \"{}\"", lfile.filename().string());
	auto const start_time     = util::timer::start();
	Texture temporary_texture = LoadTexture(lfile.string().c_str());

	if (!is_texture_valid(temporary_texture)) {
		errorlog("Invalid texture properties");
		UnloadTexture(temporary_texture);
	}

	auto shared_texture = std::shared_ptr<Texture>(new Texture(std::move(temporary_texture)), texture_deleter{});
	tracelog("Allocated shared texture | bounds: {}x{} | id: {} | address: {}", shared_texture->width,
	         shared_texture->height, shared_texture->id, fmt::ptr(shared_texture.get()));
	auto const [iterator, is_inserted] = cached_textures_.emplace(lfile, std::move(shared_texture));
	auto const end_time                = util::timer::end(start_time);

	if (!is_inserted) {
		errorlog("Couldn't insert to cache");
		return nullptr;
	} else {
		tracelog("Successfully inserted to cache | cache size: {}", cached_textures_.size());
	}

	debuglog("Done | took {}ms", end_time);
	return iterator->second;
}

// private
void TextureRepo::clear() {
	cached_textures_.clear();
}

// private
void TextureRepo::purge_unused() {
	std::erase_if(cached_textures_, [](auto const& pair) {
		return pair.second.use_count() <= 1;
	});
}

// private
std::shared_ptr<Texture> TextureRepo::try_fetch_from_cache(std::filesystem::path const& file) {
	if (auto iterator = cached_textures_.find(file); iterator != cached_textures_.end()) {
		return iterator->second;
	}

	return nullptr;
}

// private
bool TextureRepo::is_texture_valid(Texture const& texture) {
	if (texture.id < 1) {
		return false;
	}

	if (texture.width < 1 || texture.height < 1) {
		return false;
	}

	return true;
}

} // namespace ae