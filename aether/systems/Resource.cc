#include <aether/systems/Resource.hh>
#include <aether/common/log.hh>
#include <aether/common/timer.hh>
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

	if (!std::filesystem::exists(path.str)) {
		errorlog("\"{}\" doesn't exist", file);
		return nullptr;
	}

	if (
		auto it = textures_refs_.find(path.str);
		it != textures_refs_.end()
	) {
		if (auto ptr = it->second.lock()) {
			return ptr;
		}
	}

	debuglog("Loading \"{}\"", path.str);

	auto start_time = timer::start();

	Texture stack = LoadTexture(path.str.c_str());

	if (stack.id < 1) {
		errorlog("Failed");
		return nullptr;
	}

	if (stack.width < 1 || stack.height < 1) {
		UnloadTexture(Texture{.id = stack.id});
		errorlog("Invalid bounds");
		return nullptr;
	}

	auto shared = std::shared_ptr<Texture>(
		new Texture(),
		[](Texture* ptr) {
			if (ptr->id > 0) {
				UnloadTexture(Texture{.id = ptr->id});
				tracelog("Unloaded texture ({}) | id: {}", fmt::ptr(ptr), ptr->id);
			}

			delete ptr;
			ptr = nullptr;
		}
	);

	shared->id = stack.id;
	shared->width = stack.width;
	shared->height = stack.height;
	shared->mipmaps = stack.mipmaps;
	shared->format = stack.format;

	tracelog("Loaded texture ({}) | id: {} | bounds: {}x{}", fmt::ptr(shared.get()), shared->id, shared->width, shared->height);

	textures_refs_[path.str] = shared;
	tracelog("Stored to texture references | current size: {}", textures_refs_.size());

	auto end_time = timer::end(start_time);

	debuglog("Done | took {}ms", end_time);

	return shared;
}

void Resource::try_clean_refs() {
	debuglog("Attempting to clean references");

	auto start_time = timer::start();

	size_t erased = 0;
	erased += clean_texture_refs();

	auto end_time = timer::end(start_time);

	debuglog("Done | erased {} ref/s | took {}ms", erased, end_time);
}

size_t Resource::clean_texture_refs() {
	if (textures_refs_.empty()) {
		return 0;
	}

	debuglog("Cleaning texture references");

	auto start_time = timer::start();

	size_t erased = 0;

	for (auto it = textures_refs_.begin(); it != textures_refs_.end();) {
		auto& weak_ptr = it->second;

		if (weak_ptr.expired()) {
			auto name = it->first;

			it = textures_refs_.erase(it);

			erased++;

			tracelog("Erased {}", name);

			continue;
		}

		++it;
	}

	auto end_time = timer::end(start_time);

	debuglog("Done | erased {} ref/s | took {}ms", erased, end_time);

	return erased;
}

}