#include <aether/systems/Resource.hh>
#include <aether/common/log.hh>
#include <aether/common/timer.hh>
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
		auto it = textures_refs_.find(path.str);
		it != textures_refs_.end()
	) {
		log::trace(fmt::format("Found stored reference for {}", path.str));

		if (auto ptr = it->second.lock()) {
			log::trace(fmt::format("Returning reference ({})", fmt::ptr(ptr.get())));
			return ptr;
		} else {
			log::trace("Stored reference no longer exists | cleaning texture references");
			clean_texture_refs();
		}
	}

	log::info(fmt::format("Loading {}", file));

	auto start_time = timer::start();

	auto texture = Texture::make_shared(path.str.c_str());
	
	if (!texture) {
		log::error("Failed");
		return nullptr;
	}

	auto [it, placed] = textures_refs_.emplace(path.str, texture);

	if (placed) {
		log::trace(fmt::format("Stored to texture references | current size: {}", textures_refs_.size()));
	} else {
		log::warn("Failed to store to texture references");
	}

	auto end_time = timer::end(start_time);

	log::info(fmt::format("Done | took {}ms", end_time));

	return texture;
}

void Resource::clean_refs() {
	log::debug("Cleaning references");

	auto start_time = timer::start();

	size_t erased = 0;
	erased += clean_texture_refs();

	auto end_time = timer::end(start_time);

	log::debug(fmt::format("Done | erased {} ref/s | took {}ms", erased, end_time));
}

size_t Resource::clean_texture_refs() {
	if (textures_refs_.empty()) {
		return 0;
	}

	log::trace("Cleaning texture references");

	auto start_time = timer::start();

	size_t erased = 0;

	for (auto it = textures_refs_.begin(); it != textures_refs_.end();) {
		auto& weak_ptr = it->second;

		if (weak_ptr.expired()) {
			auto name = it->first;

			it = textures_refs_.erase(it);

			erased++;

			log::trace(fmt::format("Erased {}", name));

			continue;
		}

		++it;
	}

	auto end_time = timer::end(start_time);

	log::trace(fmt::format("Done | erased {} ref/s | took {}ms", erased, end_time));

	return erased;
}

}