#include <texture2d.hh>

namespace aether {

texture2d::texture2d(std::string_view file)
        : rltx_(LoadTexture(file.data())) {}

texture2d::~texture2d() {
	UnloadTexture(rltx_);
}

rltexture const& texture2d::get() const {
	return rltx_;
}

unsigned int texture2d::id() const {
	return rltx_.id;
}

size<int> texture2d::bounds() const {
	return size<int>(rltx_.width, rltx_.height);
}

} // namespace aether