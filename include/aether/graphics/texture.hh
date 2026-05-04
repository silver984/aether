#pragma once
#include <aether/math/size.hh>
#include <aether/common/pointers.hh>
#include <string>
#include <string_view>
#include <cstdint>

namespace ae {

class Texture final {
public:
    Texture();
    ~Texture();

private:
    explicit Texture(
        std::string_view file,
        uint32_t id,
        size<int> const& bounds,
        int mipmaps,
        int format
    );

public:
    Texture(Texture const&) = delete;
    Texture(Texture&&) noexcept = default;
    Texture& operator =(Texture const&) = delete;
    Texture& operator =(Texture&&) noexcept = default;

    [[nodiscard]] static sptr<Texture> load_shared(char const* file);
    [[nodiscard]] std::string_view file() const;
    [[nodiscard]] uint32_t id() const;
    [[nodiscard]] size<int> bounds() const;
    [[nodiscard]] int mipmaps() const;
    [[nodiscard]] int format() const;

private:
    std::string file_;
    uint32_t id_; // OpenGL Texture id
    size<int> bounds_;
    int mipmaps_;
    int format_;
};

}