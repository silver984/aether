#pragma once
#include <aether/util/path_map.hh>
#include <memory>

struct Texture;

namespace ae {

class Aether;
class Context;

class TextureRepo final {
	friend class Aether;

private:
	TextureRepo();

public:
	~TextureRepo();
	TextureRepo(TextureRepo const&)            = delete;
	TextureRepo(TextureRepo&&)                 = delete;
	TextureRepo& operator=(TextureRepo const&) = delete;
	TextureRepo& operator=(TextureRepo&&)      = delete;

	std::shared_ptr<Texture> fetch(std::string_view file);
	// void reserve(std::string_view file);
	// void free_reserved(std::string_view reserved);

private:
	void clear();
	void purge_unused_not_reserved();
	// void purge_reserved();
	[[nodiscard]] bool validate_texture(Texture const& texture);

	struct cached_texture final {
		cached_texture() : is_reserved(false) {}
		std::shared_ptr<Texture> texture;
		bool is_reserved;
	};

	path_map<cached_texture> cached_textures_;
};

} // namespace ae