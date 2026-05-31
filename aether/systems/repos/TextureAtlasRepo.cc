#ifdef AETHER_DEBUG
	#include <aether/debug/log.hh>
#endif
#include <aether/systems/repos/TextureAtlasRepo.hh>
#include <aether/util/filesystem.hh>
#include <aether/util/string.hh>
#include <aether/util/timer.hh>
#include <algorithm>
#include <cctype>
#include <charconv>
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
#ifdef AETHER_DEBUG
		errorlog("Filesystem gave an error");
#endif
		return nullptr;
	}

	if (auto from_cache = try_fetch_from_cache(lfile)) {
		return from_cache;
	}

	auto const file_extension = util::fs::file_extension(lfile);

	// TODO: json, plist, and txt
	if (!util::str::string_matches_any(file_extension, {".xml"})) {
#ifdef AETHER_DEBUG
		errorlog("Unsupported file format | file: \"{}\"", lfile.filename().string());
#endif
		return nullptr;
	}

	purge_unused();

#ifdef AETHER_VERBOSE_DEBUG
	debuglog("Loading \"{}\"", lfile.filename().string());
	auto const start_time = util::timer::start();
#endif

	std::shared_ptr<texture_atlas> shared_texture_atlas = nullptr;

	if (file_extension == ".xml") {
		shared_texture_atlas = xml_parse(lfile);
	}

	if (!shared_texture_atlas) {
#ifdef AETHER_DEBUG
		errorlog("Failed");
#endif
		return nullptr;
	}

	auto [iterator, _] = cached_texture_atlases_.emplace(lfile, std::move(shared_texture_atlas));

#ifdef AETHER_VERBOSE_DEBUG
	auto const end_time = util::timer::end(start_time);
	tracelog("Successfully inserted to cache | cache size: {}", cached_texture_atlases_.size());
	debuglog("Done | took {}ms", end_time);
#endif

	return iterator->second;
}

void TextureAtlasRepo::purge_unused() {
	std::erase_if(cached_texture_atlases_, [](auto& pair) {
		return pair.second.use_count() <= 1;
	});
}

// private
void TextureAtlasRepo::clear() {
	cached_texture_atlases_.clear();
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
#ifdef AETHER_DEBUG
		errorlog("Failed to load XML file");
#endif
		return nullptr;
	}

	switch (assess_xml_format(document)) {
		using enum xml_format;
	case adobe_animate: {
#ifdef AETHER_VERBOSE_DEBUG
		tracelog("Detected Adobe Animate's XML format");
#endif
		return xml_adobe_animate_parse(document);
	}
	case texture_packer: {
#ifdef AETHER_VERBOSE_DEBUG
		tracelog("Detected TexturePacker's generic XML format");
#endif
		return xml_texture_packer_parse(document);
	}
	case unknown:
	default: {
#ifdef AETHER_DEBUG
		errorlog("Unknown XML format");
#endif
		return nullptr;
	}
	}

#ifdef AETHER_DEBUG
	errorlog("Undefined error");
#endif
	return nullptr;
}

// private
std::shared_ptr<texture_atlas>
TextureAtlasRepo::xml_parse_delegate(tinyxml2::XMLDocument const& document, std::string_view element_name,
                                     std::function<void(tinyxml2::XMLElement const&, texture_atlas&)> callback) {
	tinyxml2::XMLElement const* root_element = document.FirstChildElement("TextureAtlas");

	if (!root_element) {
#ifdef AETHER_DEBUG
		errorlog("Couldn't find root element");
#endif
		return nullptr;
	}

#ifdef AETHER_VERBOSE_DEBUG
	debuglog("Parsing");
	auto const start_time = util::timer::start();
#endif

	std::shared_ptr<texture_atlas> shared_texture_atlas = std::make_shared<texture_atlas>();

#ifdef AETHER_VERBOSE_DEBUG
	tracelog("Allocated shared texture atlas | address: {}", fmt::ptr(shared_texture_atlas.get()));
#endif

	for (tinyxml2::XMLElement const* current_element = root_element->FirstChildElement(element_name.data());
	     current_element != nullptr; current_element = current_element->NextSiblingElement(element_name.data())) {
		callback(*current_element, *shared_texture_atlas);
	}

	if (shared_texture_atlas->animations.empty()) {
#ifdef AETHER_DEBUG
		errorlog("Failed | atlas has no data");
#endif
		return nullptr;
	}

	// just for logging
	std::size_t subtexture_count = 0;

	// automatically rearrange subtextures
	for (auto& [_, subtextures] : shared_texture_atlas->animations) {
		subtexture_count += subtextures.size();

		std::stable_sort(subtextures.begin(), subtextures.end(),
		                 [](texture_atlas::subtexture const& a, texture_atlas::subtexture const& b) {
			                 return a.reference_index < b.reference_index;
		                 });
	}

#ifdef AETHER_VERBOSE_DEBUG
	auto const end_time = util::timer::end(start_time);
	tracelog("Atlas data populated | animation count: {} | subtexture/frame count: {}",
	         shared_texture_atlas->animations.size(), subtexture_count);
	debuglog("Done | took {}ms", end_time);
#endif

	return shared_texture_atlas;
}

// private
std::shared_ptr<texture_atlas> TextureAtlasRepo::xml_adobe_animate_parse(tinyxml2::XMLDocument const& document) {
	return xml_parse_delegate(
	    document, "SubTexture", [this](tinyxml2::XMLElement const& current_element, texture_atlas& atlas) -> void {
		    char const* frame_name = current_element.Attribute("name");

		    if (!frame_name) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_subtexture("no name attribute");
#endif
			    return;
		    }

		    auto const parsed_frame_name = parse_frame_name(frame_name);

		    if (!parsed_frame_name.has_value()) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_subtexture("invalid name", frame_name);
#endif
			    return;
		    }

		    texture_atlas::subtexture temporary_subtexture(parsed_frame_name->second);

		    using enum tinyxml2::XMLError;
		    if (current_element.QueryIntAttribute("x", &temporary_subtexture.source_rect.x) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_subtexture("no x attribute", frame_name);
#endif
			    return;
		    }

		    if (current_element.QueryIntAttribute("y", &temporary_subtexture.source_rect.y) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_subtexture("no y attribute", frame_name);
#endif
			    return;
		    }

		    if (current_element.QueryIntAttribute("width", &temporary_subtexture.source_rect.width) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_subtexture("no width attribute", frame_name);
#endif
			    return;
		    }

		    if (current_element.QueryIntAttribute("height", &temporary_subtexture.source_rect.height) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_subtexture("no height attribute", frame_name);
#endif
			    return;
		    }

		    // these dont need to have valid values
		    // they simply fallback to their defaults
		    current_element.QueryIntAttribute("frameX", &temporary_subtexture.offsets.x);
		    current_element.QueryIntAttribute("frameY", &temporary_subtexture.offsets.y);
		    current_element.QueryBoolAttribute("rotated", &temporary_subtexture.is_rotated);

		    atlas.animations[std::string(parsed_frame_name->first)].emplace_back(std::move(temporary_subtexture));
	    });
}

std::shared_ptr<texture_atlas> TextureAtlasRepo::xml_texture_packer_parse(tinyxml2::XMLDocument const& document) {
	return xml_parse_delegate(
	    document, "sprite", [this](tinyxml2::XMLElement const& current_element, texture_atlas& atlas) -> void {
		    char const* frame_name = current_element.Attribute("n");

		    if (!frame_name) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_subtexture("no n attribute");
#endif
			    return;
		    }

		    auto const parsed_frame_name = parse_frame_name(frame_name);

		    if (!parsed_frame_name.has_value()) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_subtexture("invalid name", frame_name);
#endif
			    return;
		    }

		    texture_atlas::subtexture temporary_subtexture(parsed_frame_name->second);

		    using enum tinyxml2::XMLError;
		    if (current_element.QueryIntAttribute("x", &temporary_subtexture.source_rect.x) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_subtexture("no x attribute", frame_name);
#endif
			    return;
		    }

		    if (current_element.QueryIntAttribute("y", &temporary_subtexture.source_rect.y) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_subtexture("no y attribute", frame_name);
#endif
			    return;
		    }

		    if (current_element.QueryIntAttribute("w", &temporary_subtexture.source_rect.width) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_subtexture("no w attribute", frame_name);
#endif
			    return;
		    }

		    if (current_element.QueryIntAttribute("h", &temporary_subtexture.source_rect.height) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_subtexture("no h attribute", frame_name);
#endif
			    return;
		    }

		    atlas.animations[std::string(parsed_frame_name->first)].emplace_back(std::move(temporary_subtexture));
	    });
}

// private
TextureAtlasRepo::xml_format TextureAtlasRepo::assess_xml_format(tinyxml2::XMLDocument const& document) {
	using enum TextureAtlasRepo::xml_format;

	tinyxml2::XMLElement const* root_element = document.FirstChildElement("TextureAtlas");

	if (!root_element) {
		return unknown;
	}

	tinyxml2::XMLElement const* first_child = root_element->FirstChildElement();

	if (!first_child) {
		return unknown;
	}

	char const* first_child_name_ccptr = first_child->Name();
	std::string_view first_child_name  = first_child_name_ccptr;

	if (first_child_name == "SubTexture") {
		return adobe_animate;
	} else if (first_child_name == "sprite") {
		return texture_packer;
	}

	return unknown;
}

// private
std::optional<std::pair<std::string_view, int>> TextureAtlasRepo::parse_frame_name(std::string_view unparsed_name) {
	if (unparsed_name.empty()) {
		return std::nullopt;
	}

	std::size_t const raw_name_length = unparsed_name.size();

	// find first digit position
	std::size_t digit_start = 0;
	while (digit_start < raw_name_length && !std::isdigit(static_cast<unsigned char>(unparsed_name[digit_start]))) {
		++digit_start;
	}

	// there are no digits
	if (digit_start == raw_name_length) {
		return std::nullopt;
	}

	// there is no leading name
	if (digit_start == 0) {
		return std::nullopt;
	}

	// find post digit position
	std::size_t digit_end = digit_start;
	while (digit_end < raw_name_length && std::isdigit(static_cast<unsigned char>(unparsed_name[digit_end]))) {
		++digit_end;
	}

	int frame_index = 0;
	auto [_, error_code] =
	    std::from_chars(unparsed_name.data() + digit_start, unparsed_name.data() + digit_end, frame_index);

	if (error_code != std::errc()) {
		return std::nullopt;
	}

	// remove the separator if its there
	std::size_t prefix_end = digit_start;
	if (prefix_end > 0) {
		char c = unparsed_name[prefix_end - 1];
		if (c == '_' || c == '-' || c == '/' || c == '#') {
			--prefix_end;
		}
	}

	// return frame name, and frame index
	return std::pair<std::string_view, int>(unparsed_name.substr(0, prefix_end), frame_index);
}

#ifdef AETHER_VERBOSE_DEBUG
// private
void TextureAtlasRepo::log_defective_subtexture(std::string_view message,
                                                std::optional<std::string_view> name_attribute) {
	if (name_attribute.has_value()) {
		tracelog("Skipping subtexture with {} | on: \"{}\"", message, name_attribute.value());
		return;
	}

	tracelog("Skipping subtexture with {}", message);
}
#endif

} // namespace ae