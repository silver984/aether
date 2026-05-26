#pragma once
#include <aether/util/string_map.hh>
#include <cstdint>
#include <memory>

struct Texture;

namespace ae {

class Aether;

class TextureRepo final {
	friend class Aether;

public:
	TextureRepo();
	~TextureRepo();
	TextureRepo(TextureRepo const&)            = delete;
	TextureRepo(TextureRepo&&)                 = delete;
	TextureRepo& operator=(TextureRepo const&) = delete;
	TextureRepo& operator=(TextureRepo&&)      = delete;

	std::shared_ptr<Texture> fetch(std::string_view file);
	void reserve(std::string_view file);
	void free_reserved(std::string_view reserved);

private:
	void purge_unused();
	void purge_reserved();

	struct cached_texture final {
		cached_texture() : is_reserved(false), time_used(0.f) {}

		std::shared_ptr<Texture> texture;
		bool is_reserved;
		float time_used;
	};

	string_map<cached_texture> cached_textures_;
};

} // namespace ae