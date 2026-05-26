#pragma once
#include <aether/common/string_map.hh>
#include <memory>
#include <string_view>

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

private:
	string_map<std::shared_ptr<Texture>> cache_;
};

} // namespace ae