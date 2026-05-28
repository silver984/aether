#include <aether/debug/log.hh>
#include <aether/systems/repos/TextureAtlasRepo.hh>
#include <aether/util/filesystem.hh>
#include <aether/util/string.hh>
#include <aether/util/timer.hh>
#include <algorithm>
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

	// TODO: json and plist
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
                                     std::function<void(tinyxml2::XMLElement const&, texture_atlas&)>&& callback) {
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

	if (shared_texture_atlas->subtextures.empty()) {
		errorlog("Failed | atlas has no valid frames");
		return nullptr;
	}

	// automatically rearrange subtextures
	for (auto& [_, subtextures] : shared_texture_atlas->subtextures) {
		std::sort(subtextures.begin(), subtextures.end(),
		          [](texture_atlas::subtexture const& a, texture_atlas::subtexture const& b) {
			          return a.reference_index < b.reference_index;
		          });
	}

	auto const end_time = util::timer::end(start_time);
	debuglog("Done | took {}ms", end_time);

	return shared_texture_atlas;
}

// private
std::shared_ptr<texture_atlas> TextureAtlasRepo::xml_adobe_animate_parse(tinyxml2::XMLDocument const& document) {
	return xml_parse_delegate(
	    document, "SubTexture", [this](tinyxml2::XMLElement const& current_element, texture_atlas& atlas) -> void {
		    char const* full_animation_name_ccptr = current_element.Attribute("name");

		    if (!full_animation_name_ccptr) {
			    log_defective_subtexture("no name attribute");
			    return;
		    }

		    std::string_view full_animation_name = full_animation_name_ccptr;

		    // adobe animate exports frame indices as exactly 4 trailing digits:
		    // examples: "idle0000", "run0023"
		    //
		    // we intentionally do NOT scan backwards until a non-digit character,
		    // because animation names themselves may legally end in digits:
		    // "idle1" + "0000" -> "idle10000"
		    // "idle2" + "0000" -> "idle20000"
		    if (full_animation_name.size() < 4) {
			    log_defective_subtexture("insufficient name length", full_animation_name);
			    return;
		    }

		    int animation_frame_index               = 0;
		    std::size_t const animation_name_length = full_animation_name.size() - 4;

		    { // parse frame index
			    std::string_view frame_index_str = full_animation_name.substr(animation_name_length);
			    char const* begin                = frame_index_str.data();
			    char const* end                  = begin + frame_index_str.size();
			    auto [ptr, error_code]           = std::from_chars(begin, end, animation_frame_index);

			    if (error_code != std::errc{} || ptr != end) {
				    log_defective_subtexture("invalid frame index", full_animation_name);
				    return;
			    }
		    }

		    texture_atlas::subtexture temporary_subtexture(animation_frame_index);

		    using enum tinyxml2::XMLError;
		    if (current_element.QueryIntAttribute("width", &temporary_subtexture.source_rect.width) != XML_SUCCESS) {
			    log_defective_subtexture("no width attribute", full_animation_name);
			    return;
		    }

		    if (current_element.QueryIntAttribute("height", &temporary_subtexture.source_rect.height) != XML_SUCCESS) {
			    log_defective_subtexture("no height attribute", full_animation_name);
			    return;
		    }

		    // these dont need to have valid values
		    // they simply fallback to their defaults
		    current_element.QueryIntAttribute("x", &temporary_subtexture.source_rect.x);
		    current_element.QueryIntAttribute("y", &temporary_subtexture.source_rect.y);
		    current_element.QueryIntAttribute("frameX", &temporary_subtexture.offsets.x);
		    current_element.QueryIntAttribute("frameY", &temporary_subtexture.offsets.y);
		    current_element.QueryBoolAttribute("rotated", &temporary_subtexture.is_rotated);

		    std::string animation_name = std::string(full_animation_name.substr(0, animation_name_length));
		    atlas.subtextures[animation_name].emplace_back(std::move(temporary_subtexture));
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