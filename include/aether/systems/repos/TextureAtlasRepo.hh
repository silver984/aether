#pragma once
#include <aether/graphic/texture_atlas.hh>
#include <aether/util/path_map.hh>
#include <functional>
#include <memory>
#include <optional>

namespace tinyxml2 {

class XMLDocument;
class XMLElement;

} // namespace tinyxml2

namespace ae {

class Aether;

class TextureAtlasRepo final {
	friend class Aether;

	enum class xml_format : int { unknown = -1, adobe_animate, texture_packer };

private:
	TextureAtlasRepo();

public:
	~TextureAtlasRepo();
	TextureAtlasRepo(TextureAtlasRepo const&)            = delete;
	TextureAtlasRepo(TextureAtlasRepo&&)                 = delete;
	TextureAtlasRepo& operator=(TextureAtlasRepo const&) = delete;
	TextureAtlasRepo& operator=(TextureAtlasRepo&&)      = delete;

	std::shared_ptr<texture_atlas> fetch(std::string_view file);
	void purge_unused();

private:
	void clear();
	[[nodiscard]] std::shared_ptr<texture_atlas> try_fetch_from_cache(std::filesystem::path const& file);
	[[nodiscard]] std::shared_ptr<texture_atlas> xml_parse(std::filesystem::path const& file);

	[[nodiscard]] std::shared_ptr<texture_atlas>
	xml_parse_delegate(tinyxml2::XMLDocument const& document, std::string_view element_name,
	                   std::function<void(tinyxml2::XMLElement const&, texture_atlas&)>&& callback);

	[[nodiscard]] std::shared_ptr<texture_atlas> xml_adobe_animate_parse(tinyxml2::XMLDocument const& document);
	[[nodiscard]] std::shared_ptr<texture_atlas> xml_texture_packer_parse(tinyxml2::XMLDocument const& document);
	[[nodiscard]] xml_format assess_xml_format(tinyxml2::XMLDocument const& document);
#if defined(AETHER_DEBUG) && defined(AETHER_VERBOSE_LOGS)
	void log_defective_subtexture(std::string_view message,
	                              std::optional<std::string_view> name_attribute = std::nullopt);
#endif

	util::path_map<std::shared_ptr<texture_atlas>> cached_texture_atlases_;
};

} // namespace ae