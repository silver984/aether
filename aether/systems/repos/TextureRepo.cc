#include <aether/debug/log.hh>
#include <aether/systems/Context.hh>
#include <aether/systems/repos/TextureRepo.hh>
#include <aether/util/filesystem.hh>
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
	std::filesystem::path file_path;

	if (auto opt_path = fs::normalized_filepath(file); opt_path.has_value()) {
		file_path = opt_path.value();
	} else {
		errorlog("Filesystem gave error");
		return nullptr;
	}

	if (auto iterator = cached_textures_.find(file_path); iterator != cached_textures_.end()) {
		return iterator->second.texture;
	}

	// TODO: support other formats than just PNG
	if (auto const file_extension = fs::file_extension(file_path); file_extension != "png") {
		errorlog("Unsupported file format | file: \"{}\"", file_path.filename().string());
		return nullptr;
	}

	purge_unused_not_reserved();

	Texture temporary_texture = LoadTexture(file_path.string().c_str());

	if (!validate_texture(temporary_texture)) {
		errorlog("Invalid texture properties");
		UnloadTexture(temporary_texture);
	}

	cached_texture temporary_cache;
	temporary_cache.texture = std::shared_ptr<Texture>(new Texture(std::move(temporary_texture)), texture_deleter{});
	auto [iterator, is_inserted] = cached_textures_.emplace(file_path, std::move(temporary_cache));

	if (!is_inserted) {
		errorlog("Couldn't insert to cache");
		return nullptr;
	}

	return iterator->second.texture;
}

// private
void TextureRepo::clear() {
	cached_textures_.clear();
}

// private
void TextureRepo::purge_unused_not_reserved() {
	for (auto iterator = cached_textures_.begin(); iterator != cached_textures_.end();) {
		if (iterator->second.is_reserved) {
			++iterator;
			continue;
		}

		if (iterator->second.texture.use_count() <= 1) {
			iterator = cached_textures_.erase(iterator);
			continue;
		}

		++iterator;
	}
}

// private
bool TextureRepo::validate_texture(Texture const& texture) {
	if (texture.id < 1) {
		return false;
	}

	if (texture.width < 1 || texture.height < 1) {
		return false;
	}

	return true;
}

} // namespace ae