#pragma once
#include <aether/math/size.hh>
#include <cstdint>

namespace ae {

struct texture final {
    ~texture();

    static texture load(char const* file_path);

    uint32_t id = 0; // OpenGL texture id
    size<int> bounds;
    int mipmaps = 0;
    int format = 0;
};

}