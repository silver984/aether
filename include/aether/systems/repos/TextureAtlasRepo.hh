#pragma once
#include <aether/graphic/texture_atlas.hh>
#include <aether/util/path_map.hh>
#include <memory>

namespace ae {

class Aether;

class TextureAtlasRepo final {
	friend class Aether;

private:
	TextureAtlasRepo();

public:
	~TextureAtlasRepo();
	TextureAtlasRepo(TextureAtlasRepo const&)            = delete;
	TextureAtlasRepo(TextureAtlasRepo&&)                 = delete;
	TextureAtlasRepo& operator=(TextureAtlasRepo const&) = delete;
	TextureAtlasRepo& operator=(TextureAtlasRepo&&)      = delete;

private:
	path_map<std::shared_ptr<texture_atlas>> cached_texture_atlases_;
};

} // namespace ae