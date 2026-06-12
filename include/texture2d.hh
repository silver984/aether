#pragma once
#include <math/size.hh>
#include <raylib.h>
#include <string_view>

using rltexture = Texture;

namespace aether {

class texture2d final {
public:
	texture2d(std::string_view file);
	~texture2d();

	rltexture const& get() const;
	unsigned int id() const;
	size<int> bounds() const;

private:
	rltexture rltx_;
};

} // namespace aether