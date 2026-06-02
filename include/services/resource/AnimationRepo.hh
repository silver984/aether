#pragma once
#include <functional>
#include <graphic/animation_map.hh>
#include <memory>
#include <optional>
#include <util/path_map.hh>
#include <utility>

namespace tinyxml2 {

class XMLDocument;
class XMLElement;

} // namespace tinyxml2

namespace aether {

class Aether;

class AnimationRepo final {
	friend class Aether;

	enum class xml_format : int { unknown = -1, adobe_animate, texture_packer };

private:
	AnimationRepo();

public:
	~AnimationRepo();
	AnimationRepo(AnimationRepo const&)            = delete;
	AnimationRepo(AnimationRepo&&)                 = delete;
	AnimationRepo& operator=(AnimationRepo const&) = delete;
	AnimationRepo& operator=(AnimationRepo&&)      = delete;

	std::shared_ptr<animation_map> fetch(std::string_view file);
	void purge_unused();

private:
	void clear();
	[[nodiscard]] std::shared_ptr<animation_map> try_fetch_from_cache(std::filesystem::path const& file);
	[[nodiscard]] std::shared_ptr<animation_map> xml_parse(std::filesystem::path const& file);

	[[nodiscard]] std::shared_ptr<animation_map>
	xml_parse_delegate(tinyxml2::XMLDocument const& document, std::string_view element_name,
	                   std::function<void(tinyxml2::XMLElement const&, animation_map&)> callback);

	[[nodiscard]] std::shared_ptr<animation_map> xml_adobe_animate_parse(tinyxml2::XMLDocument const& document);
	[[nodiscard]] std::shared_ptr<animation_map> xml_texture_packer_parse(tinyxml2::XMLDocument const& document);
	[[nodiscard]] xml_format assess_xml_format(tinyxml2::XMLDocument const& document);
	[[nodiscard]] std::string parse_frame_name(std::string_view unparsed_name);

#ifdef AETHER_VERBOSE_DEBUG
	void log_defective_subtexture(std::string_view message,
	                              std::optional<std::string_view> name_attribute = std::nullopt);
#endif

	util::path_map<std::shared_ptr<animation_map>> cache_;
};

} // namespace ae