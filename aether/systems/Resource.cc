#include <aether/systems/Resource.hh>
#include <aether/common/log.hh>
#include <fmt/format.h>
#include <raylib.h>
#include <filesystem>
#include <string>
#include <cctype>

namespace {

struct file_path final {
	static file_path parse(std::string_view file) {
		std::filesystem::path abs = std::filesystem::absolute(file);
		std::string ext = abs.extension().string();

		if (!ext.empty() && ext[0] == '.') {
			// remove the dot from the extension
			ext.erase(0, 1);
		}

		// make extension lowercase
		std::transform(ext.begin(), ext.end(), ext.begin(), tolower);

		return {
			.str = abs.string(),
			.dir = abs.parent_path().string(),
			.name = abs.stem().string(),
			.ext = ext
		};
	}

	std::string str;
	std::string dir;
	std::string name;
	std::string ext;
};

}

namespace ae {

// private
Resource::Resource() = default;

// private
Resource::~Resource() = default;

std::shared_ptr<Texture> Resource::load_shared_texture(std::string_view file) {
	file_path path = file_path::parse(file);

	// TODO: check format validity

	if (
		auto it = textures_.find(path.str);
		it != textures_.end()
	) {
		log::trace(fmt::format("Found cache for {}", path.str));

		if (auto ptr = it->second.lock()) {
			log::trace(fmt::format("Returning found cache ({})", fmt::ptr(ptr.get())));
			return ptr;
		}
	}

	log::info(fmt::format("Loading {}", file));

	auto texture = Texture::make_shared(path.str.c_str());
	
	if (!texture) {
		log::error("Failed");
		return nullptr;
	}

	auto texture_bounds = texture->bounds();
	log::trace(fmt::format("Made shared texture ({}) | id: {} | bounds: {}x{}",
		fmt::ptr(texture.get()),
		texture->id(),
		texture_bounds.width,
		texture_bounds.height
	));

	auto [it, placed] = textures_.emplace(path.str, texture);

	if (placed) {
		log::debug("Stored to cache");
		log::info("Loaded");
		return texture;
	} else {
		log::warn("Failed to store to cache");
	}

	return texture;
}

void Resource::clean_cache() {
	log::debug("Cleaning cache");

	size_t erased = 0;

	for (auto it = textures_.begin(); it != textures_.end();) {
		auto& weak_ptr = it->second;

		if (weak_ptr.expired()) {
			auto name = it->first;

			it = textures_.erase(it);

			erased++;
			
			log::trace(fmt::format("Erased {}", name));

			continue;
		}

		++it;
	}

	log::debug(fmt::format("Done | erased {} slot/s", erased));
}

}