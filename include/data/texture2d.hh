#pragma once
#include <math/size.hh>
#include <raylib.h>
#include <string_view>

using rltexture2d = Texture;

namespace aether {

class texture2d final {
public:
	texture2d(std::string_view file);
	~texture2d();
	texture2d(texture2d const&)            = delete;
	texture2d(texture2d&&)                 = delete;
	texture2d& operator=(texture2d const&) = delete;
	texture2d& operator=(texture2d&&)      = delete;

	rltexture2d const& get() const;
	unsigned int id() const;
	size<int> bounds() const;

private:
	rltexture2d rltx_;
};

} // namespace aether