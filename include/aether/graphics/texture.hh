#ifndef __AETHER_GRAPHICS_TEXTURE_HH__
#define __AETHER_GRAPHICS_TEXTURE_HH__

#include <aether/math/size.hh>
#include <memory>
#include <string>
#include <string_view>

namespace ae {

class Texture final {
public:
    Texture();
    explicit Texture(
        std::string_view file,
        unsigned int id,
        size<int> bounds,
        int mipmaps,
        int format
    );

    ~Texture();

public:
    Texture(Texture const&) = delete;
    Texture(Texture&&) noexcept = default;
    Texture& operator=(Texture const&) = delete;
    Texture& operator=(Texture&&) noexcept = default;

    [[nodiscard]] static std::shared_ptr<Texture> make_shared(char const* file);
    [[nodiscard]] std::string_view file() const;
    [[nodiscard]] unsigned int id() const;
    [[nodiscard]] size<int> bounds() const;
    [[nodiscard]] int mipmaps() const;
    [[nodiscard]] int format() const;

private:
    std::string file_;
    unsigned int id_; // OpenGL Texture id
    size<int> bounds_;
    int mipmaps_;
    int format_;
};

}

#endif