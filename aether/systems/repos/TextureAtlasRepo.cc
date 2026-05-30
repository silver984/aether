#include <aether/debug/log.hh>
#include <aether/systems/repos/TextureAtlasRepo.hh>
#include <aether/util/filesystem.hh>
#include <aether/util/string.hh>
#include <aether/util/timer.hh>
#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstddef>
#include <tinyxml2/tinyxml2.h>

#if !(defined(AETHER_DEBUG) && defined(AETHER_VERBOSE_LOGS))
	#define log_defective_subtexture(...) ((void)0)
#endif

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

	// TODO: json, plist, and txt
	if (!util::str::string_matches_any(file_extension, {".xml"})) {
		errorlog("Unsupported file format | file: \"{}\"", lfile.filename().string());
		return nullptr;
	}

	purge_unused();

	debuglog("Loading \"{}\"", lfile.filename().string());
	auto const start_time = util::timer::start();

	std::shared_ptr<texture_atlas> shared_texture_atlas = nullptr;

	if (file_extension == ".xml") {
		shared_texture_atlas = xml_parse(lfile);
	}

	if (!shared_texture_atlas) {
		errorlog("Failed");
		return nullptr;
	}

	auto [iterator, _] = cached_texture_atlases_.emplace(lfile, std::move(shared_texture_atlas));
	tracelog("Successfully inserted to cache | cache size: {}", cached_texture_atlases_.size());

	auto const end_time = util::timer::end(start_time);
	debuglog("Done | took {}ms", end_time);
	return iterator->second;
}

void TextureAtlasRepo::purge_unused() {
	std::erase_if(cached_texture_atlases_, [](auto const& pair) {
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
		errorlog("Failed to load XML file");
		return nullptr;
	}

	switch (assess_xml_format(document)) {
		using enum xml_format;
	case adobe_animate: {
		tracelog("Detected Adobe Animate XML format");
		return xml_adobe_animate_parse(document);
	}
	case texture_packer: {
		tracelog("Detected TexturePacker generic XML format");
		return xml_texture_packer_parse(document);
	}
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
std::shared_ptr<texture_atlas>
TextureAtlasRepo::xml_parse_delegate(tinyxml2::XMLDocument const& document, std::string_view element_name,
                                     std::function<void(tinyxml2::XMLElement const&, texture_atlas&)> callback) {
	tinyxml2::XMLElement const* root_element = document.FirstChildElement("TextureAtlas");

	if (!root_element) {
		errorlog("Couldn't find root element");
		return nullptr;
	}

	debuglog("Parsing");
	auto const start_time = util::timer::start();

	std::shared_ptr<texture_atlas> shared_texture_atlas = std::make_shared<texture_atlas>();
	tracelog("Allocated shared texture atlas | address: {}", fmt::ptr(shared_texture_atlas.get()));

	for (tinyxml2::XMLElement const* current_element = root_element->FirstChildElement(element_name.data());
	     current_element != nullptr; current_element = current_element->NextSiblingElement(element_name.data())) {
		callback(*current_element, *shared_texture_atlas);
	}

	if (shared_texture_atlas->animations.empty()) {
		errorlog("Failed | atlas has no data");
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

	tracelog("Atlas data populated | animation count: {} | subtexture/frame count: {}",
	         shared_texture_atlas->animations.size(), subtexture_count);

	auto const end_time = util::timer::end(start_time);
	debuglog("Done | took {}ms", end_time);

	return shared_texture_atlas;
}

// private
std::shared_ptr<texture_atlas> TextureAtlasRepo::xml_adobe_animate_parse(tinyxml2::XMLDocument const& document) {
	return xml_parse_delegate(
	    document, "SubTexture", [this](tinyxml2::XMLElement const& current_element, texture_atlas& atlas) -> void {
		    char const* full_frame_name = current_element.Attribute("name");

		    if (!full_frame_name) {
			    log_defective_subtexture("no name attribute");
			    return;
		    }

		    auto const parsed_frame_name = parse_frame_name(full_frame_name);

		    if (!parsed_frame_name.has_value()) {
			    log_defective_subtexture("invalid name", full_frame_name);
			    return;
		    }

		    texture_atlas::subtexture temporary_subtexture(parsed_frame_name->second);

		    using enum tinyxml2::XMLError;
		    if (current_element.QueryIntAttribute("width", &temporary_subtexture.source_rect.width) != XML_SUCCESS) {
			    log_defective_subtexture("no width attribute", full_frame_name);
			    return;
		    }

		    if (current_element.QueryIntAttribute("height", &temporary_subtexture.source_rect.height) != XML_SUCCESS) {
			    log_defective_subtexture("no height attribute", full_frame_name);
			    return;
		    }

		    // these dont need to have valid values
		    // they simply fallback to their defaults
		    current_element.QueryIntAttribute("x", &temporary_subtexture.source_rect.x);
		    current_element.QueryIntAttribute("y", &temporary_subtexture.source_rect.y);
		    current_element.QueryIntAttribute("frameX", &temporary_subtexture.offsets.x);
		    current_element.QueryIntAttribute("frameY", &temporary_subtexture.offsets.y);
		    current_element.QueryBoolAttribute("rotated", &temporary_subtexture.is_rotated);

		    atlas.animations[std::string(parsed_frame_name->first)].emplace_back(std::move(temporary_subtexture));
	    });
}

std::shared_ptr<texture_atlas> TextureAtlasRepo::xml_texture_packer_parse(tinyxml2::XMLDocument const& document) {
	return xml_parse_delegate(document, "sprite",
	                          [this](tinyxml2::XMLElement const& current_element, texture_atlas& atlas) -> void {
		                          // TODO
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
	std::size_t first_digit_position  = 0;

	// find first digit position
	while (first_digit_position < raw_name_length &&
	       !std::isdigit(static_cast<unsigned char>(unparsed_name[first_digit_position]))) {
		++first_digit_position;
	}

	// there are no non digits
	if (first_digit_position == 0) {
		return std::nullopt;
	}

	// no digits at all
	if (first_digit_position == raw_name_length) {
		return std::nullopt;
	}

	std::size_t post_digit_position = first_digit_position;

	// find post digit position
	while (post_digit_position < raw_name_length &&
	       std::isdigit(static_cast<unsigned char>(unparsed_name[post_digit_position]))) {
		++post_digit_position;
	}

	int frame_index      = 0;
	auto [_, error_code] = std::from_chars(unparsed_name.data() + first_digit_position,
	                                       unparsed_name.data() + post_digit_position, frame_index);

	if (error_code != std::errc()) {
		return std::nullopt;
	}

	return std::pair<std::string_view, int>(unparsed_name.substr(0, first_digit_position), frame_index);
}

#if defined(AETHER_DEBUG) && defined(AETHER_VERBOSE_LOGS)
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