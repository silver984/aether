#pragma once
#include <aether/graphic/texture_atlas.hh>
#include <aether/util/path_map.hh>
#include <memory>

namespace tinyxml2 {

class XMLDocument;

}

namespace ae {

class Aether;

class TextureAtlasRepo final {
	friend class Aether;

	enum class xml_formats : int { unknown = -1, adobe_animate, texture_packer };

private:
	TextureAtlasRepo();

public:
	~TextureAtlasRepo();
	TextureAtlasRepo(TextureAtlasRepo const&)            = delete;
	TextureAtlasRepo(TextureAtlasRepo&&)                 = delete;
	TextureAtlasRepo& operator=(TextureAtlasRepo const&) = delete;
	TextureAtlasRepo& operator=(TextureAtlasRepo&&)      = delete;

	std::shared_ptr<texture_atlas> fetch(std::string_view file);

private:
	[[nodiscard]] std::shared_ptr<texture_atlas> try_fetch_from_cache(std::filesystem::path const& file);
	[[nodiscard]] std::shared_ptr<texture_atlas> xml_parse(std::filesystem::path const& file);
	[[nodiscard]] xml_formats assess_xml_format(tinyxml2::XMLDocument const& document);

	util::path_map<std::shared_ptr<texture_atlas>> cached_texture_atlases_;
};

} // namespace ae