#ifndef __AETHER_SYSTEMS_RESOURCE_HH__
#define __AETHER_SYSTEMS_RESOURCE_HH__

#include <aether/common/string_map.hh>
#include <aether/graphic/texture_atlas.hh>
#include <cstdint>
#include <memory>
#include <string_view>

struct Texture;

namespace ae {

class Funkin;
class Director;

class Resource final {
	friend class Funkin;
	friend class Director;

  public:
	Resource();
	~Resource();
	Resource(Resource const&)            = delete;
	Resource(Resource&&)                 = delete;
	Resource& operator=(Resource const&) = delete;
	Resource& operator=(Resource&&)      = delete;

	[[nodiscard]] std::shared_ptr<Texture> load_shared_texture(std::string_view file);
	[[nodiscard]] std::shared_ptr<texture_atlas> load_shared_texture_atlas(std::string_view path,
	                                                                       std::string_view image_format,
	                                                                       std::string_view data_format);

  private:
	void try_clean_refs();
#if defined(AETHER_DEBUG) && defined(AETHER_VERBOSE_LOGS)
	size_t clean_texture_refs();
	size_t clean_texture_atlas_refs();
#else
	void clean_texture_refs();
	void clean_texture_atlas_refs();
#endif

	string_map<std::weak_ptr<Texture>> texture_wrefs_;
	string_map<std::weak_ptr<texture_atlas>> texture_atlas_wrefs_;
};

} // namespace ae

#endif