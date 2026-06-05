#pragma once
#include <data/animation_map.hh>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace tinyxml2 {

class XMLDocument;
class XMLElement;

} // namespace tinyxml2

namespace aether {

class Aether;

class AnimationRepository final {
	friend class Aether;

	enum xml_format : int { unknown = -1, adobe_animate, texture_packer };

private:
	AnimationRepository();

public:
	~AnimationRepository();
	AnimationRepository(AnimationRepository const&)            = delete;
	AnimationRepository(AnimationRepository&&)                 = delete;
	AnimationRepository& operator=(AnimationRepository const&) = delete;
	AnimationRepository& operator=(AnimationRepository&&)      = delete;

	std::shared_ptr<animation_map> fetch(std::string_view file);
	void purge_unused();

private:
	void clear_cache();
	[[nodiscard]] std::shared_ptr<animation_map> try_fetch_from_cache(std::filesystem::path const& file) const;
	[[nodiscard]] std::shared_ptr<animation_map> xml_parse(std::filesystem::path const& file);

	[[nodiscard]] std::shared_ptr<animation_map>
	xml_parse_delegate(tinyxml2::XMLDocument const& document, std::string_view element_name,
	                   std::function<void(tinyxml2::XMLElement const&, animation_map&)>&& callback);

	[[nodiscard]] std::shared_ptr<animation_map> xml_adobe_animate_parse(tinyxml2::XMLDocument const& document);
	[[nodiscard]] std::shared_ptr<animation_map> xml_texture_packer_parse(tinyxml2::XMLDocument const& document);
	[[nodiscard]] xml_format assess_xml_format(tinyxml2::XMLDocument const& document) const;
	[[nodiscard]] std::string parse_frame_name(std::string_view unparsed_name) const;

#ifdef AETHER_VERBOSE_DEBUG
	void log_defective_frame(std::string_view message, std::optional<std::string_view> name = std::nullopt) const;
#endif

	std::unordered_map<std::filesystem::path, std::shared_ptr<animation_map>> cache_;
};

} // namespace aether