#include <aether/common/log.hh>
#include <aether/common/timer.hh>
#include <aether/systems/Resource.hh>
#include <cctype>
#include <external/tinyxml2.h>
#include <filesystem>
#include <fmt/format.h>
#include <raylib.h>
#include <string>
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

		return {.str = abs.string(),
				.dir = abs.parent_path().string(),
				.name = abs.stem().string(),
				.ext = ext};
	}

	std::string str;
	std::string dir;
	std::string name;
	std::string ext;
};

#if defined(AETHER_DEBUG) && defined(AETHER_VERBOSE_LOGS)
template <typename T>
size_t cleaning_helper(ae::string_map<T>& map) {
	size_t erased = 0;

	for (auto it = map.begin(); it != map.end();) {
		auto& weak_ptr = it->second;

		if (weak_ptr.expired()) {
			it = map.erase(it);
			erased++;
			continue;
		}

		++it;
	}

	return erased;
}
#else
template <typename T>
void cleaning_helper(ae::string_map<T>& map) {
	for (auto it = map.begin(); it != map.end();) {
		auto& weak_ptr = it->second;

		if (weak_ptr.expired()) {
			it = map.erase(it);
			continue;
		}

		++it;
	}
}
#endif

} // namespace

namespace ae {

// private
Resource::Resource() = default;

// private
Resource::~Resource() = default;

std::shared_ptr<Texture> Resource::load_shared_texture(std::string_view file) {
	file_path lfile = file_path::parse(file);

	// TODO: check format validity

	if (auto it = texture_wrefs_.find(lfile.str); it != texture_wrefs_.end()) {
		if (auto ptr = it->second.lock()) {
			return ptr;
		}

		texture_wrefs_.erase(it);
	}

	debuglog("Loading texture | file: \"{}\"", lfile.str);

	if (!std::filesystem::exists(lfile.str)) {
		errorlog("File doesn't exist");
		return nullptr;
	}

	auto start_time = timer::start();

	Texture tmp = LoadTexture(lfile.str.c_str());

	if (tmp.id < 1) {
		errorlog("Failed");
		return nullptr;
	}

	if (tmp.width < 1 || tmp.height < 1) {
		UnloadTexture(tmp);
		errorlog("Invalid bounds");
		return nullptr;
	}

	auto shared = std::shared_ptr<Texture>(new Texture(std::move(tmp)), texture_deleter{});
	tracelog("Loaded texture ({}) | OpenGL id: {} | bounds: {}x{}", fmt::ptr(shared.get()), shared->id, shared->width, shared->height);

	texture_wrefs_[lfile.str] = shared;
	tracelog("Stored to texture references | current size: {}", texture_wrefs_.size());

	auto end_time = timer::end(start_time);
	debuglog("Done | took {}ms", end_time);

	return shared;
}

std::shared_ptr<texture_atlas> Resource::load_shared_texture_atlas(std::string_view path, std::string_view image_format, std::string_view data_format) {
	file_path lpath = file_path::parse(path);

	if (auto it = texture_atlas_wrefs_.find(lpath.str); it != texture_atlas_wrefs_.end()) {
		if (auto ptr = it->second.lock()) {
			return ptr;
		}

		texture_atlas_wrefs_.erase(it);
	}

	debuglog("Loading texture atlas | path: \"{}\"", lpath.str);

	auto load_start_time = timer::start();

	auto shared = std::make_shared<texture_atlas>();
	shared->texture = load_shared_texture(fmt::format("{}.{}", path, image_format));

	if (!shared->texture) {
		errorlog("Failed");
		return nullptr;
	}

	// TODO: support for other data formats other than XML
	// this currently only support adobe animate, aim to also support texturepacker

	file_path data_path = file_path::parse(fmt::format("{}.{}", path, data_format));

	if (!std::filesystem::exists(data_path.str)) {
		errorlog("Data file doesn't exist");
		return nullptr;
	}

	namespace txml = tinyxml2;

	txml::XMLDocument doc;

	if (txml::XMLError res = doc.LoadFile(data_path.str.c_str());
		res != txml::XMLError::XML_SUCCESS) {
		errorlog("Failed to load XML file");
		return nullptr;
	}

	txml::XMLElement* root = doc.FirstChildElement("TextureAtlas");

	if (!root) {
		errorlog("XML is corrupted or is in an invalid format");
		return nullptr;
	}

	debuglog("Proceeding to parse subtextures");

	auto parse_start_time = timer::start();

	for (tinyxml2::XMLElement* elem = root->FirstChildElement("SubTexture");
		 elem != nullptr; elem = elem->NextSiblingElement("SubTexture")) {
		char const* full_anim_name_ccptr = elem->Attribute("name");

		if (!full_anim_name_ccptr) {
			tracelog("Skipping subtexture with no name attribute");
			continue;
		}

		std::string full_anim_name = full_anim_name_ccptr;

		if (full_anim_name.size() < 4) {
			tracelog("Skipping subtexture with insufficient name size | name: \"{}\"", full_anim_name);
			continue;
		}

		std::string anim_name = full_anim_name.substr(0, full_anim_name.size() - 4);

		int index{};

		try {
			index = std::stoi(full_anim_name.substr(full_anim_name.size() - 4));
		} catch (...) {
			tracelog("Skipping subtexture name with invalid frame index | name: \"{}\"", full_anim_name);
			continue;
		}

		texture_atlas::subtexture tmp(index);

		if (txml::XMLError res = elem->QueryIntAttribute("x", &tmp.source_rect.x);
			res != txml::XMLError::XML_SUCCESS) {
			tracelog("Skipping subtexture with no x attribute | name: \"{}\"", full_anim_name);
			continue;
		}

		if (txml::XMLError res = elem->QueryIntAttribute("y", &tmp.source_rect.y);
			res != txml::XMLError::XML_SUCCESS) {
			tracelog("Skipping subtexture with no y attribute | name: \"{}\"", full_anim_name);
			continue;
		}

		if (txml::XMLError res = elem->QueryIntAttribute("width", &tmp.source_rect.width);
			res != txml::XMLError::XML_SUCCESS) {
			tracelog("Skipping subtexture with no width attribute | name: \"{}\"", full_anim_name);
			continue;
		}

		if (txml::XMLError res = elem->QueryIntAttribute("height", &tmp.source_rect.height);
			res != txml::XMLError::XML_SUCCESS) {
			tracelog("Skipping subtexture with no height attribute | name: \"{}\"", full_anim_name);
			continue;
		}

		elem->QueryIntAttribute("frameX", &tmp.transform_offset.x);
		elem->QueryIntAttribute("frameY", &tmp.transform_offset.y);

		shared->subtextures[anim_name].emplace_back(std::move(tmp));
	}

	size_t frame_count = 0; // just for logging

	for (auto& [_, vec] : shared->subtextures) {
		frame_count += vec.size();

		std::sort(vec.begin(), vec.end(),
				  [](texture_atlas::subtexture const& a, texture_atlas::subtexture const& b) {
					  return a.reference_index < b.reference_index;
				  });
	}

	auto parse_end_time = timer::end(parse_start_time);
	debuglog("Parsing done | took {}ms", parse_end_time);

	tracelog("Loaded texture atlas ({}) | texture: {} | animation count: {} | frame count: {}",
			 fmt::ptr(shared.get()), fmt::ptr(shared->texture.get()), shared->subtextures.size(), frame_count);

	texture_atlas_wrefs_[lpath.str] = shared;
	tracelog("Stored to texture atlas references | current size: {}", texture_atlas_wrefs_.size());

	auto load_end_time = timer::end(load_start_time);
	debuglog("Loading done | took {}ms", load_end_time);

	return shared;
}

void Resource::try_clean_refs() {
#if defined(AETHER_DEBUG) && defined(AETHER_VERBOSE_LOGS)
	debuglog("Attempting to clean references");

	auto start_time = timer::start();

	size_t erased = 0;
	erased += clean_texture_refs();
	erased += clean_texture_atlas_refs();

	auto end_time = timer::end(start_time);

	debuglog("Done | erased {} ref/s | took {}ms", erased, end_time);
#else
	clean_texture_refs();
	clean_texture_atlas_refs();
#endif
}

#if defined(AETHER_DEBUG) && defined(AETHER_VERBOSE_LOGS)
size_t Resource::clean_texture_refs() {
	if (texture_wrefs_.empty()) {
		return 0;
	}

	debuglog("Cleaning texture references");

	auto start_time = timer::start();
	size_t erased = cleaning_helper(texture_wrefs_);
	auto end_time = timer::end(start_time);

	debuglog("Done | erased {} ref/s | took {}ms", erased, end_time);

	return erased;
}

size_t Resource::clean_texture_atlas_refs() {
	if (texture_atlas_wrefs_.empty()) {
		return 0;
	}

	debuglog("Cleaning texture atlas references");

	auto start_time = timer::start();
	size_t erased = cleaning_helper(texture_atlas_wrefs_);
	auto end_time = timer::end(start_time);

	debuglog("Done | erased {} ref/s | took {}ms", erased, end_time);

	return erased;
}
#else
void Resource::clean_texture_refs() {
	if (texture_wrefs_.empty()) {
		return;
	}

	cleaning_helper(texture_wrefs_);
}

void Resource::clean_texture_atlas_refs() {
	if (texture_atlas_wrefs_.empty()) {
		return;
	}

	cleaning_helper(texture_atlas_wrefs_);
}
#endif

} // namespace ae