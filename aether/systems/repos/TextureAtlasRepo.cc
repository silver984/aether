#include <aether/debug/log.hh>
#include <aether/systems/repos/TextureAtlasRepo.hh>
#include <aether/util/filesystem.hh>
#include <aether/util/string.hh>
#include <algorithm>
#include <cstddef>
#include <optional>
#include <tinyxml2/tinyxml2.h>

namespace ae {

// private
TextureAtlasRepo::TextureAtlasRepo()  = default;
TextureAtlasRepo::~TextureAtlasRepo() = default;

std::shared_ptr<texture_atlas> TextureAtlasRepo::fetch(std::string_view file) {
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

	auto const file_extension = util::fs::file_extension(lfile);

	// TODO: json and plist
	if (!util::str::string_matches_any(file_extension, {".xml" /* .json, .plist */})) {
		errorlog("Unsupported file format | file: \"{}\"", lfile.filename().string());
		return nullptr;
	}

	std::shared_ptr<texture_atlas> shared_texture_atlas = nullptr;

	if (file_extension == ".xml") {
		shared_texture_atlas = xml_parse(lfile);
	}

	if (!shared_texture_atlas) {
		errorlog("Failed | file: \"{}\"", lfile.filename().string());
		return nullptr;
	}

	auto const [iterator, _] = cached_texture_atlases_.emplace(lfile, std::move(shared_texture_atlas));
	return iterator->second;
}

// private
std::shared_ptr<texture_atlas> TextureAtlasRepo::try_fetch_from_cache(std::filesystem::path const& file) {
	if (auto iterator = cached_texture_atlases_.find(file); iterator != cached_texture_atlases_.end()) {
		return iterator->second;
	}

	return nullptr;
}

// private
std::shared_ptr<texture_atlas> TextureAtlasRepo::xml_parse(std::filesystem::path const& file) {
	tinyxml2::XMLDocument document;

	using enum tinyxml2::XMLError;
	if (document.LoadFile(file.string().c_str()) != XML_SUCCESS) {
		errorlog("Failed to load XML file");
		return nullptr;
	}

	switch (assess_xml_format(document)) {
		using enum xml_format;
	case adobe_animate: return xml_adobe_animate_parse(document);
	case unknown:
	default: {
		errorlog("Unknown XML format");
		return nullptr;
	}
	}

	errorlog("Undefined error");
	return nullptr;
}

// private
std::shared_ptr<texture_atlas> TextureAtlasRepo::xml_adobe_animate_parse(tinyxml2::XMLDocument const& document) {
	auto shared_texture_atlas     = std::make_shared<texture_atlas>();
	auto const* root              = document.FirstChildElement("TextureAtlas");
	auto log_defective_subtexture = [](std::string_view message,
	                                   std::optional<std::string_view> animation_name = std::nullopt) -> void {
		if (animation_name.has_value()) {
			tracelog("Skipping subtexture with {} | name: \"{}\"", message, animation_name);
			return;
		}

		tracelog("Skipping subtexture with {}", message);
	};

	for (auto const* element = root->FirstChildElement("SubTexture"); element != nullptr;
	     element             = element->NextSiblingElement("SubTexture")) {
		char const* full_animation_name_ccptr = element->Attribute("name");

		if (!full_animation_name_ccptr) {
			log_defective_subtexture("no name attribute");
			continue;
		}

		std::string_view full_animation_name = full_animation_name_ccptr;

		if (full_animation_name.size() < 4) {
			log_defective_subtexture("insufficient name length", full_animation_name);
			continue;
		}

		// adobe animate exports frame indices as exactly 4 trailing digits:
		// examples: "idle0000", "run0023"
		//
		// we intentionally do NOT scan backwards until a non-digit character,
		// because animation names themselves may legally end in digits:
		// "idle1" + "0000" -> "idle10000"
		// "idle2" + "0000" -> "idle20000"
		auto const animation_name_length = full_animation_name.size() - 4;
		int animation_frame_index        = 0;

		try {
			animation_frame_index = std::stoi(std::string(full_animation_name.substr(animation_name_length)));
		} catch (...) {
			log_defective_subtexture("invalid frame index", full_animation_name);
			continue;
		}

		texture_atlas::subtexture temporary_subtexture(animation_frame_index);

		using enum tinyxml2::XMLError;
		if (element->QueryIntAttribute("width", &temporary_subtexture.source_rect.width) != XML_SUCCESS) {
			log_defective_subtexture("no width attribute", full_animation_name);
			continue;
		}

		if (element->QueryIntAttribute("height", &temporary_subtexture.source_rect.height) != XML_SUCCESS) {
			log_defective_subtexture("no height attribute", full_animation_name);
			continue;
		}

		// these dont need to have valid values
		// they simply fallback to their defaults
		element->QueryIntAttribute("x", &temporary_subtexture.source_rect.x);
		element->QueryIntAttribute("y", &temporary_subtexture.source_rect.y);
		element->QueryIntAttribute("frameX", &temporary_subtexture.offsets.x);
		element->QueryIntAttribute("frameY", &temporary_subtexture.offsets.y);
		element->QueryBoolAttribute("rotated", &temporary_subtexture.is_rotated);

		std::string_view animation_name = full_animation_name.substr(0, animation_name_length);
		auto const [iterator, _]        = shared_texture_atlas->subtextures.emplace(animation_name);
		iterator->second.emplace_back(std::move(temporary_subtexture));
	}

	// rearrange subtextures
	for (auto& [_, subtextures] : shared_texture_atlas->subtextures) {
		std::sort(subtextures.begin(), subtextures.end(),
		          [](texture_atlas::subtexture const& a, texture_atlas::subtexture const& b) {
			          return a.reference_index < b.reference_index;
		          });
	}

	return shared_texture_atlas;
}

// private
TextureAtlasRepo::xml_format TextureAtlasRepo::assess_xml_format(tinyxml2::XMLDocument const& document) {
	using enum TextureAtlasRepo::xml_format;

	auto const* root = document.FirstChildElement("TextureAtlas");

	if (!root) {
		return unknown;
	}

	auto const* first_child = root->FirstChildElement();

	if (!first_child) {
		return unknown;
	}

	char const* first_child_name_ccptr     = first_child->Name();
	std::string_view first_child_name_strv = first_child_name_ccptr;

	if (first_child_name_strv == "SubTexture") {
		return adobe_animate;
	} else if (first_child_name_strv == "sprite") {
		return texture_packer;
	}

	return unknown;
}

} // namespace ae