#include <aether/animation_repository.hh>
#include <aether/filesystem.hh>
#include <aether/log.hh>
#include <aether/string.hh>
#include <aether/timer.hh>
#include <cctype>
#include <cstddef>
#include <tinyxml2/tinyxml2.h>

#ifndef AETHER_VERBOSE_DEBUG
	#define log_defective_frame_(...) ((void)0)
#endif

namespace aether {

animation_repository::animation_repository()  = default;
animation_repository::~animation_repository() = default;

strong_ref<animation_map> animation_repository::fetch(std::string_view file) {
	std::filesystem::path lfile = std::filesystem::weakly_canonical(file);

	if (!std::filesystem::exists(lfile)) {
		AETHER_ENGINE_ERRORLOG("File doesn't exist | file: \"{}\"", file);
		return nullptr;
	}

	if (auto from_cache = try_fetch_from_cache_(lfile)) {
		return from_cache;
	}

	auto const file_extension = file_extension(lfile);

	// todo: json, plist, and txt
	if (!string_matches_any(file_extension, {".xml"})) {
		AETHER_ENGINE_ERRORLOG("Unsupported file format ? file: \"{}\"", lfile.filename().string());
		return nullptr;
	}

	purge_unused();

	AETHER_ENGINE_DEBUGLOG("Loading \"{}\"", lfile.filename().string());
	timer t;
	t.start();

	strong_ref<animation_map> shared_map = nullptr;

	if (file_extension == ".xml") {
		shared_map = xml_parse_(lfile);
	}

	if (!shared_map) {
		AETHER_ENGINE_ERRORLOG("Failed");
		return nullptr;
	}

	auto const [it, _] = cache_.emplace(lfile, std::move(shared_map));
	t.stop();
	AETHER_ENGINE_TRACELOG("Successfully inserted to cache ? cache size: {}", cache_.size());
	AETHER_ENGINE_DEBUGLOG("Done ({}ms)", t.duration());

	return it->second;
}

void animation_repository::purge_unused() {
	std::erase_if(cache_, [](auto const& pair) {
		return pair.second.strong_count() <= 1;
	});
}

void animation_repository::clear_cache_() {
	cache_.clear();
}

strong_ref<animation_map> animation_repository::try_fetch_from_cache_(std::filesystem::path const& file) const {
	if (auto const it = cache_.find(file); it != cache_.end()) {
		return it->second;
	}
	return nullptr;
}

strong_ref<animation_map> animation_repository::xml_parse_(std::filesystem::path const& file) {
	tinyxml2::XMLDocument document;

	using enum tinyxml2::XMLError;
	if (document.LoadFile(file.string().c_str()) != XML_SUCCESS) {
		AETHER_ENGINE_ERRORLOG("Failed to load XML file");
		return nullptr;
	}

	switch (assess_xml_format_(document)) {
		using enum xml_format;
	case adobe_animate: {
		AETHER_ENGINE_TRACELOG("Detected Adobe Animate's XML format");
		return xml_adobe_animate_parse_(document);
	}
	case texture_packer: {
		AETHER_ENGINE_TRACELOG("Detected TexturePacker's generic XML format");
		return xml_texture_packer_parse_(document);
	}
	case unknown:
	default: {
		AETHER_ENGINE_ERRORLOG("Unknown XML format");
		return nullptr;
	}
	}

	AETHER_ENGINE_ERRORLOG("Undefined error");
	return nullptr;
}

strong_ref<animation_map>
animation_repository::xml_parse_delegate_(tinyxml2::XMLDocument const& document, std::string_view element_name,
                                          std::function<void(tinyxml2::XMLElement const&, animation_map&)>&& callback) {
	tinyxml2::XMLElement const* root_element = document.FirstChildElement("TextureAtlas");

	if (!root_element) {
		AETHER_ENGINE_ERRORLOG("Couldn't find root element");
		return nullptr;
	}

	AETHER_ENGINE_DEBUGLOG("Parsing");
	timer t;
	t.start();

	strong_ref<animation_map> shared_map = new animation_map();
	AETHER_ENGINE_TRACELOG("Allocated shared animation map ? address: {}", fmt::ptr(shared_map.get()));

	char const* const element_name_data = element_name.data();
	for (tinyxml2::XMLElement const* current_element = root_element->FirstChildElement(element_name_data); current_element != nullptr;
	     current_element                             = current_element->NextSiblingElement(element_name_data)) {
		callback(*current_element, *shared_map);
	}

	if (shared_map->empty()) {
		AETHER_ENGINE_ERRORLOG("Failed ? map has no data");
		return nullptr;
	}

	t.stop();

#ifndef AETHER_ENGINE_RELWITHDEB
	size_t frame_count = 0;
	for (auto& [_, data] : *shared_map) {
		frame_count += data.frames.size();
	}
	AETHER_ENGINE_TRACELOG("Map populated ? count: {}, frames: {}", shared_map->size(), frame_count);
#endif
	AETHER_ENGINE_DEBUGLOG("Done ({}ms)", t.duration());

	return shared_map;
}

strong_ref<animation_map> animation_repository::xml_adobe_animate_parse_(tinyxml2::XMLDocument const& document) {
	return xml_parse_delegate_(document, "SubTexture", [this](tinyxml2::XMLElement const& current_element, animation_map& map) -> void {
		char const* frame_name = current_element.Attribute("name");

		if (!frame_name) {
			log_defective_frame_("no name attribute");
			return;
		}

		std::string const parsed_frame_name = parse_frame_name_(frame_name);

		if (parsed_frame_name.empty()) {
			log_defective_frame_("invalid name", frame_name);
			return;
		}

		atlas_region frame;

		using enum tinyxml2::XMLError;
		if (current_element.QueryIntAttribute("x", &frame.source_rect.x) != XML_SUCCESS) {
			log_defective_frame_("no x attribute", frame_name);
			return;
		}

		if (current_element.QueryIntAttribute("y", &frame.source_rect.y) != XML_SUCCESS) {
			log_defective_frame_("no y attribute", frame_name);
			return;
		}

		if (current_element.QueryIntAttribute("width", &frame.source_rect.width) != XML_SUCCESS) {
			log_defective_frame_("no width attribute", frame_name);
			return;
		}

		if (current_element.QueryIntAttribute("height", &frame.source_rect.height) != XML_SUCCESS) {
			log_defective_frame_("no height attribute", frame_name);
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

strong_ref<animation_map> animation_repository::xml_texture_packer_parse_(tinyxml2::XMLDocument const& document) {
	return xml_parse_delegate_(document, "sprite", [this](tinyxml2::XMLElement const& current_element, animation_map& map) -> void {
		char const* frame_name = current_element.Attribute("n");

		if (!frame_name) {
			log_defective_frame_("no n attribute");
			return;
		}

		std::string const parsed_frame_name = parse_frame_name_(frame_name);

		if (parsed_frame_name.empty()) {
			log_defective_frame_("invalid name", frame_name);
			return;
		}

		atlas_region frame;

		using enum tinyxml2::XMLError;
		if (current_element.QueryIntAttribute("x", &frame.source_rect.x) != XML_SUCCESS) {
			log_defective_frame_("no x attribute", frame_name);
			return;
		}

		if (current_element.QueryIntAttribute("y", &frame.source_rect.y) != XML_SUCCESS) {
			log_defective_frame_("no y attribute", frame_name);
			return;
		}

		if (current_element.QueryIntAttribute("w", &frame.source_rect.width) != XML_SUCCESS) {
			log_defective_frame_("no w attribute", frame_name);
			return;
		}

		if (current_element.QueryIntAttribute("h", &frame.source_rect.height) != XML_SUCCESS) {
			log_defective_frame_("no h attribute", frame_name);
			return;
		}

		map[parsed_frame_name].frames.emplace_back(std::move(frame));
	});
}

animation_repository::xml_format animation_repository::assess_xml_format_(tinyxml2::XMLDocument const& document) const {
	tinyxml2::XMLElement const* root_element = document.FirstChildElement("TextureAtlas");

	if (!root_element) {
		return unknown;
	}

	tinyxml2::XMLElement const* first_child = root_element->FirstChildElement();

	if (!first_child) {
		return unknown;
	}

	char const* name_c_str      = first_child->Name();
	std::string_view child_name = name_c_str;

	if (child_name == "SubTexture") {
		return adobe_animate;
	} else if (child_name == "sprite") {
		return texture_packer;
	}

	return unknown;
}

std::string animation_repository::parse_frame_name_(std::string_view unparsed_name) const {
	if (unparsed_name.empty()) {
		return {};
	}

	size_t const name_length = unparsed_name.size();

	// find first digit position
	size_t digit_start = 0;
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
	size_t prefix_end = digit_start;
	if (prefix_end > 0) {
		char const c = unparsed_name[prefix_end - 1];
		if (c == '_' || c == '-' || c == '/' || c == '#') {
			--prefix_end;
		}
	}

	// return frame name
	return std::string(unparsed_name.substr(0, prefix_end));
}

#ifndef AETHER_ENGINE_RELWITHDEB
void animation_repository::log_defective_frame_(std::string_view message, std::optional<std::string_view> name) const {
	if (name.has_value()) {
		AETHER_ENGINE_TRACELOG("Skipping frame with {} ? on: \"{}\"", message, name.value());
		return;
	}
	AETHER_ENGINE_TRACELOG("Skipping frame with {}", message);
}
#endif

} // namespace aether