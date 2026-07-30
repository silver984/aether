#pragma once
#include <cstdint>
#include <data/animation_map.hh>
#include <filesystem>
#include <functional>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <util/ref.hh>

namespace tinyxml2 {

class XMLDocument;
class XMLElement;

} // namespace tinyxml2

namespace aether {

class game;

class animation_repository final {
	friend class game;

	enum xml_format : int8_t {
		unknown       = -1,
		adobe_animate = 1,
		texture_packer
	};

public:
	~animation_repository();
	strong_ref<animation_map> fetch(std::string_view file);
	void purge_unused();

private:
	animation_repository();
	void clear_cache_();
	[[nodiscard]] strong_ref<animation_map> try_fetch_from_cache_(std::filesystem::path const& file) const;
	[[nodiscard]] strong_ref<animation_map> xml_parse_(std::filesystem::path const& file);
	[[nodiscard]] strong_ref<animation_map>
	xml_parse_delegate_(tinyxml2::XMLDocument const& document, std::string_view element_name,
	                    std::function<void(tinyxml2::XMLElement const&, animation_map&)>&& callback);
	[[nodiscard]] strong_ref<animation_map> xml_adobe_animate_parse_(tinyxml2::XMLDocument const& document);
	[[nodiscard]] strong_ref<animation_map> xml_texture_packer_parse_(tinyxml2::XMLDocument const& document);
	[[nodiscard]] xml_format assess_xml_format_(tinyxml2::XMLDocument const& document) const;
	[[nodiscard]] std::string parse_frame_name_(std::string_view unparsed_name) const;
#ifndef AE_RELWITHDEB
	void log_defective_frame_(std::string_view message, std::optional<std::string_view> name = std::nullopt) const;
#endif

	std::unordered_map<std::filesystem::path, strong_ref<animation_map>> cache_;
};

} // namespace aether