#include <aether/debug/log.hh>
#include <aether/systems/Resource.hh>
#include <aether/util/timer.hh>
#include <cctype>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <raylib.h>
#include <tinyxml2/tinyxml2.h>
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
		std::string extension     = abs.extension().string();

		if (!extension.empty() && extension[0] == '.') {
			// remove the dot from the extension
			extension.erase(0, 1);
		}

		// make extension lowercase
		std::transform(extension.begin(), extension.end(), extension.begin(), tolower);

		return {.str = abs.string(), .dir = abs.parent_path().string(), .name = abs.stem().string(), .ext = extension};
	}

	std::string const str;
	std::string const dir;
	std::string const name;
	std::string const ext;
};

#if defined(AETHER_DEBUG) && defined(AETHER_VERBOSE_LOGS)
template <typename T>
std::size_t cleaning_helper(ae::string_map<T>& map) {
	std::size_t erased = 0;

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

// std::filesystem::path base_game_assets_path;

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

	auto start_time = util::timer::start();

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

	tracelog("Loaded texture ({}) | OpenGL id: {} | bounds: {}x{}", fmt::ptr(shared.get()), shared->id, shared->width,
	         shared->height);

	texture_wrefs_[lfile.str] = shared;

	tracelog("Stored to texture references | current size: {}", texture_wrefs_.size());

	auto end_time = util::timer::end(start_time);
	debuglog("Done | took {}ms", end_time);

	return shared;
}

std::shared_ptr<texture_atlas> Resource::load_shared_texture_atlas(std::string_view path, std::string_view image_format,
                                                                   std::string_view data_format) {
	file_path lpath = file_path::parse(path);

	if (auto it = texture_atlas_wrefs_.find(lpath.str); it != texture_atlas_wrefs_.end()) {
		if (auto ptr = it->second.lock()) {
			return ptr;
		}

		texture_atlas_wrefs_.erase(it);
	}

	debuglog("Loading texture atlas | path: \"{}\"", lpath.str);

	auto load_start_time = util::timer::start();

	auto shared     = std::make_shared<texture_atlas>();
	shared->texture = load_shared_texture(fmt::format("{}.{}", path, image_format));

	if (!shared->texture) {
		errorlog("Failed");
		return nullptr;
	}

	// TODO: support for other data formats other than XML
	// this currently only support adobe animate, aim to also support texturepacker
	if (data_format != "xml") {
		errorlog("Unidentified data format");
		return nullptr;
	}

	file_path data_path = file_path::parse(fmt::format("{}.{}", path, data_format));

	if (!std::filesystem::exists(data_path.str)) {
		errorlog("Data file doesn't exist");
		return nullptr;
	}

	tinyxml2::XMLDocument doc;

	if (tinyxml2::XMLError res = doc.LoadFile(data_path.str.c_str()); res != tinyxml2::XMLError::XML_SUCCESS) {
		errorlog("Failed to load XML file");
		return nullptr;
	}

	tinyxml2::XMLElement* root = doc.FirstChildElement("TextureAtlas");

	if (!root) {
		errorlog("Couldn't find first child element inside XML");
		return nullptr;
	}

	texture_atlas_formats atlas_format = validate_xml_texture_atlas(root);

	debuglog("Proceeding to parse subtextures");

	auto parse_start_time = util::timer::start();

	for (tinyxml2::XMLElement* elem = root->FirstChildElement("SubTexture"); elem != nullptr;
	     elem                       = elem->NextSiblingElement("SubTexture")) {
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
		int index             = 0;

		try {
			index = std::stoi(full_anim_name.substr(full_anim_name.size() - 4));
		} catch (...) {
			tracelog("Skipping subtexture name with invalid frame index | name: \"{}\"", full_anim_name);
			continue;
		}

		texture_atlas::subtexture tmp(index);

		if (tinyxml2::XMLError res = elem->QueryIntAttribute("x", &tmp.source_rect.x);
		    res != tinyxml2::XMLError::XML_SUCCESS) {
			tracelog("Skipping subtexture with no x attribute | name: \"{}\"", full_anim_name);
			continue;
		}

		if (tinyxml2::XMLError res = elem->QueryIntAttribute("y", &tmp.source_rect.y);
		    res != tinyxml2::XMLError::XML_SUCCESS) {
			tracelog("Skipping subtexture with no y attribute | name: \"{}\"", full_anim_name);
			continue;
		}

		if (tinyxml2::XMLError res = elem->QueryIntAttribute("width", &tmp.source_rect.width);
		    res != tinyxml2::XMLError::XML_SUCCESS) {
			tracelog("Skipping subtexture with no width attribute | name: \"{}\"", full_anim_name);
			continue;
		}

		if (tinyxml2::XMLError res = elem->QueryIntAttribute("height", &tmp.source_rect.height);
		    res != tinyxml2::XMLError::XML_SUCCESS) {
			tracelog("Skipping subtexture with no height attribute | name: \"{}\"", full_anim_name);
			continue;
		}

		elem->QueryIntAttribute("frameX", &tmp.offsets.x);
		elem->QueryIntAttribute("frameY", &tmp.offsets.y);
		elem->QueryBoolAttribute("rotated", &tmp.is_rotated);

		shared->subtextures[anim_name].emplace_back(std::move(tmp));
	}

	std::size_t frame_count = 0; // just for logging

	for (auto& [_, vec] : shared->subtextures) {
		frame_count += vec.size();

		std::sort(vec.begin(), vec.end(), [](texture_atlas::subtexture const& a, texture_atlas::subtexture const& b) {
			return a.reference_index < b.reference_index;
		});
	}

	auto parse_end_time = util::timer::end(parse_start_time);
	debuglog("Parsing done | took {}ms", parse_end_time);

	tracelog("Loaded texture atlas ({}) | texture: {} | animation count: {} | frame count: {}", fmt::ptr(shared.get()),
	         fmt::ptr(shared->texture.get()), shared->subtextures.size(), frame_count);

	texture_atlas_wrefs_[lpath.str] = shared;

	tracelog("Stored to texture atlas references | current size: {}", texture_atlas_wrefs_.size());

	auto load_end_time = util::timer::end(load_start_time);
	debuglog("Loading done | took {}ms", load_end_time);

	return shared;
}

// private
void Resource::try_clean_refs() {
#if defined(AETHER_DEBUG) && defined(AETHER_VERBOSE_LOGS)
	debuglog("Attempting to clean references");

	auto start_time = util::timer::start();

	std::size_t erased = 0;
	erased += clean_texture_refs();
	erased += clean_texture_atlas_refs();

	auto end_time = util::timer::end(start_time);

	debuglog("Done | erased {} ref/s | took {}ms", erased, end_time);
#else
	clean_texture_refs();
	clean_texture_atlas_refs();
#endif
}

#if defined(AETHER_DEBUG) && defined(AETHER_VERBOSE_LOGS)
// private
std::size_t Resource::clean_texture_refs() {
	if (texture_wrefs_.empty()) {
		return 0;
	}

	debuglog("Cleaning texture references");

	auto start_time    = util::timer::start();
	std::size_t erased = cleaning_helper(texture_wrefs_);
	auto end_time      = util::timer::end(start_time);

	debuglog("Done | erased {} ref/s | took {}ms", erased, end_time);

	return erased;
}

// private
std::size_t Resource::clean_texture_atlas_refs() {
	if (texture_atlas_wrefs_.empty()) {
		return 0;
	}

	debuglog("Cleaning texture atlas references");

	auto start_time    = util::timer::start();
	std::size_t erased = cleaning_helper(texture_atlas_wrefs_);
	auto end_time      = util::timer::end(start_time);

	debuglog("Done | erased {} ref/s | took {}ms", erased, end_time);

	return erased;
}
#else
// private
void Resource::clean_texture_refs() {
	if (texture_wrefs_.empty()) {
		return;
	}

	cleaning_helper(texture_wrefs_);
}

// private
void Resource::clean_texture_atlas_refs() {
	if (texture_atlas_wrefs_.empty()) {
		return;
	}

	cleaning_helper(texture_atlas_wrefs_);
}
#endif

Resource::texture_atlas_formats Resource::validate_xml_texture_atlas(tinyxml2::XMLElement* const& root) {
	return texture_atlas_formats::__none__;
}

} // namespace ae