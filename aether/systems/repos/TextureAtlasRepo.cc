#include <aether/debug/log.hh>
#include <aether/systems/repos/TextureAtlasRepo.hh>
#include <aether/util/filesystem.hh>
#include <aether/util/string.hh>
#include <cstddef>
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

	if (!util::str::string_matches_any(file_extension, {".xml"})) {
		errorlog("Unsupported file format | file: \"{}\"", lfile.filename().string());
		return nullptr;
	}

	if (file_extension == ".xml") {
	}

	return nullptr;
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

	if (document.LoadFile(file.string().c_str()) != tinyxml2::XMLError::XML_SUCCESS) {
		errorlog("Failed to load XML file");
		return nullptr;
	}

	xml_formats const format = assess_xml_format(document);
	switch (format) {
		using enum xml_formats;
	case unknown:
	default: return nullptr;
	}

	errorlog("Undefined error");
	return nullptr;
}

TextureAtlasRepo::xml_formats TextureAtlasRepo::assess_xml_format(tinyxml2::XMLDocument const& document) {
	using enum TextureAtlasRepo::xml_formats;

	tinyxml2::XMLElement const* root = document.FirstChildElement("TextureAtlas");

	if (!root) {
		return unknown;
	}

	tinyxml2::XMLElement const* first_child = root->FirstChildElement();

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