#pragma once
#include <aether/graphic/texture_atlas.hh>
#include <aether/util/string_map.hh>
#include <cstddef>
#include <memory>

struct Texture;

namespace tinyxml2 {

class XMLElement;

}

namespace ae {

class Aether;
class Director;

class Resource final {
	friend class Aether;
	friend class Director;

public:
	Resource();
	~Resource();
	Resource(Resource const&)            = delete;
	Resource(Resource&&)                 = delete;
	Resource& operator=(Resource const&) = delete;
	Resource& operator=(Resource&&)      = delete;

	[[nodiscard]] std::shared_ptr<Texture> load_shared_texture(std::string_view file);
	[[nodiscard]] std::shared_ptr<texture_atlas>
	load_shared_texture_atlas(std::string_view path, std::string_view image_format, std::string_view data_format);

private:
	enum class texture_atlas_formats : int { adobe_animate, texture_packer, __none__ };

	void try_clean_refs();
#if defined(AETHER_DEBUG) && defined(AETHER_VERBOSE_LOGS)
	size_t clean_texture_refs();
	size_t clean_texture_atlas_refs();
#else
	void clean_texture_refs();
	void clean_texture_atlas_refs();
#endif
	texture_atlas_formats validate_xml_texture_atlas(tinyxml2::XMLElement* const& root);

	string_map<std::weak_ptr<Texture>> texture_wrefs_;
	string_map<std::weak_ptr<texture_atlas>> texture_atlas_wrefs_;
};

} // namespace ae