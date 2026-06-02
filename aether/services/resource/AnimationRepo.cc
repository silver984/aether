#ifdef AETHER_DEBUG
	#include <log.hh>
#endif
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <services/resource/AnimationRepo.hh>
#include <tinyxml2/tinyxml2.h>
#include <util/filesystem.hh>
#include <util/string.hh>
#include <util/timer.hh>

namespace aether {

// private
AnimationRepo::AnimationRepo()  = default;
AnimationRepo::~AnimationRepo() = default;

std::shared_ptr<animation_map> AnimationRepo::fetch(std::string_view file) {
	std::filesystem::path lfile;

	if (auto const optional_file = util::normalized_filepath(file); optional_file.has_value()) {
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

	auto const file_extension = util::file_extension(lfile);

	// TODO: json, plist, and txt
	if (!util::string_matches_any(file_extension, {".xml"})) {
#ifdef AETHER_DEBUG
		errorlog("Unsupported file format | file: \"{}\"", lfile.filename().string());
#endif
		return nullptr;
	}

	purge_unused();

#ifdef AETHER_VERBOSE_DEBUG
	debuglog("Loading \"{}\"", lfile.filename().string());
	auto const start_time = util::start();
#endif

	std::shared_ptr<animation_map> shared_map;

	if (file_extension == ".xml") {
		shared_map = xml_parse(lfile);
	}

	if (!shared_map) {
#ifdef AETHER_DEBUG
		errorlog("Failed");
#endif
		return nullptr;
	}

	auto const [iterator, _] = cache_.emplace(lfile, std::move(shared_map));

#ifdef AETHER_VERBOSE_DEBUG
	auto const end_time = util::end(start_time);
	tracelog("Successfully inserted to cache | cache size: {}", cache_.size());
	debuglog("Done | took {}ms", end_time);
#endif

	return iterator->second;
}

void AnimationRepo::purge_unused() {
	std::erase_if(cache_, [](auto& pair) {
		return pair.second.use_count() <= 1;
	});
}

// private
void AnimationRepo::clear() {
	cache_.clear();
}

// private
std::shared_ptr<animation_map> AnimationRepo::try_fetch_from_cache(std::filesystem::path const& file) {
	if (auto const iterator = cache_.find(file); iterator != cache_.end()) {
		return iterator->second;
	}

	return nullptr;
}

// private
std::shared_ptr<animation_map> AnimationRepo::xml_parse(std::filesystem::path const& file) {
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
std::shared_ptr<animation_map>
AnimationRepo::xml_parse_delegate(tinyxml2::XMLDocument const& document, std::string_view element_name,
                                  std::function<void(tinyxml2::XMLElement const&, animation_map&)> callback) {
	tinyxml2::XMLElement const* root_element = document.FirstChildElement("TextureAtlas");

	if (!root_element) {
#ifdef AETHER_DEBUG
		errorlog("Couldn't find root element");
#endif
		return nullptr;
	}

#ifdef AETHER_VERBOSE_DEBUG
	debuglog("Parsing");
	auto const start_time = util::start();
#endif

	std::shared_ptr<animation_map> shared_map = std::make_shared<animation_map>();

#ifdef AETHER_VERBOSE_DEBUG
	tracelog("Allocated shared animation map | address: {}", fmt::ptr(shared_map.get()));
#endif

	char const* const element_name_ccptr = element_name.data();
	for (tinyxml2::XMLElement const* current_element = root_element->FirstChildElement(element_name_ccptr);
	     current_element != nullptr; current_element = current_element->NextSiblingElement(element_name_ccptr)) {
		callback(*current_element, *shared_map);
	}

	if (shared_map->empty()) {
#ifdef AETHER_DEBUG
		errorlog("Failed | map has no data");
#endif
		return nullptr;
	}

#ifdef AETHER_VERBOSE_DEBUG
	auto const end_time = util::end(start_time);

	std::size_t frame_count = 0;
	for (auto& [_, data] : *shared_map) {
		frame_count += data.frames.size();
	}

	tracelog("Map populated | count: {} | frames: {}", shared_map->size(), frame_count);
	debuglog("Done | took {}ms", end_time);
#endif

	return shared_map;
}

// private
std::shared_ptr<animation_map> AnimationRepo::xml_adobe_animate_parse(tinyxml2::XMLDocument const& document) {
	return xml_parse_delegate(
	    document, "SubTexture", [this](tinyxml2::XMLElement const& current_element, animation_map& map) -> void {
		    char const* frame_name = current_element.Attribute("name");

		    if (!frame_name) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_frame("no name attribute");
#endif
			    return;
		    }

		    std::string const parsed_frame_name = parse_frame_name(frame_name);

		    if (parsed_frame_name.empty()) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_frame("invalid name", frame_name);
#endif
			    return;
		    }

		    atlas_region frame;

		    using enum tinyxml2::XMLError;
		    if (current_element.QueryIntAttribute("x", &frame.source_rect.x) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_frame("no x attribute", frame_name);
#endif
			    return;
		    }

		    if (current_element.QueryIntAttribute("y", &frame.source_rect.y) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_frame("no y attribute", frame_name);
#endif
			    return;
		    }

		    if (current_element.QueryIntAttribute("width", &frame.source_rect.width) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_frame("no width attribute", frame_name);
#endif
			    return;
		    }

		    if (current_element.QueryIntAttribute("height", &frame.source_rect.height) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_frame("no height attribute", frame_name);
#endif
			    return;
		    }

		    // these dont need to have valid values
		    // they simply fallback to their defaults
		    current_element.QueryIntAttribute("frameX", &frame.offsets.x);
		    current_element.QueryIntAttribute("frameY", &frame.offsets.y);
		    current_element.QueryBoolAttribute("rotated", &frame.is_rotated);

		    map[parsed_frame_name].frames.emplace_back(std::move(frame));
	    });
}

std::shared_ptr<animation_map> AnimationRepo::xml_texture_packer_parse(tinyxml2::XMLDocument const& document) {
	return xml_parse_delegate(
	    document, "sprite", [this](tinyxml2::XMLElement const& current_element, animation_map& map) -> void {
		    char const* frame_name = current_element.Attribute("n");

		    if (!frame_name) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_frame("no n attribute");
#endif
			    return;
		    }

		    std::string const parsed_frame_name = parse_frame_name(frame_name);

		    if (parsed_frame_name.empty()) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_frame("invalid name", frame_name);
#endif
			    return;
		    }

		    atlas_region frame;

		    using enum tinyxml2::XMLError;
		    if (current_element.QueryIntAttribute("x", &frame.source_rect.x) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_frame("no x attribute", frame_name);
#endif
			    return;
		    }

		    if (current_element.QueryIntAttribute("y", &frame.source_rect.y) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_frame("no y attribute", frame_name);
#endif
			    return;
		    }

		    if (current_element.QueryIntAttribute("w", &frame.source_rect.width) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_frame("no w attribute", frame_name);
#endif
			    return;
		    }

		    if (current_element.QueryIntAttribute("h", &frame.source_rect.height) != XML_SUCCESS) {
#ifdef AETHER_VERBOSE_DEBUG
			    log_defective_frame("no h attribute", frame_name);
#endif
			    return;
		    }

		    map[parsed_frame_name].frames.emplace_back(std::move(frame));
	    });
}

// private
AnimationRepo::xml_format AnimationRepo::assess_xml_format(tinyxml2::XMLDocument const& document) {
	tinyxml2::XMLElement const* root_element = document.FirstChildElement("TextureAtlas");

	if (!root_element) {
		return unknown;
	}

	tinyxml2::XMLElement const* first_child = root_element->FirstChildElement();

	if (!first_child) {
		return unknown;
	}

	char const* name_ptr        = first_child->Name();
	std::string_view child_name = name_ptr;

	if (child_name == "SubTexture") {
		return adobe_animate;
	} else if (child_name == "sprite") {
		return texture_packer;
	}

	return unknown;
}

// private
std::string AnimationRepo::parse_frame_name(std::string_view unparsed_name) {
	if (unparsed_name.empty()) {
		return {};
	}

	std::size_t const name_length = unparsed_name.size();

	// find first digit position
	std::size_t digit_start = 0;
	while (digit_start < name_length && !std::isdigit(unparsed_name[digit_start])) {
		++digit_start;
	}

	// there are no digits
	if (digit_start == name_length) {
		return {};
	}

	// there is no leading name
	if (digit_start == 0) {
		return {};
	}

	// remove the separator if its there
	std::size_t prefix_end = digit_start;
	if (prefix_end > 0) {
		char const c = unparsed_name[prefix_end - 1];
		if (c == '_' || c == '-' || c == '/' || c == '#') {
			--prefix_end;
		}
	}

	// return frame name
	return std::string(unparsed_name.substr(0, prefix_end));
}

#ifdef AETHER_VERBOSE_DEBUG
// private
void AnimationRepo::log_defective_frame(std::string_view message, std::optional<std::string_view> name) {
	if (name.has_value()) {
		tracelog("Skipping frame with {} | on: \"{}\"", message, name.value());
		return;
	}

	tracelog("Skipping frame with {}", message);
}
#endif

} // namespace aether