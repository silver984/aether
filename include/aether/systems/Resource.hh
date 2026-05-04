#pragma once
#include <aether/common/string_map.hh>
#include <aether/common/pointers.hh>
#include <aether/graphics/Texture.hh>
#include <string_view>

namespace ae {

class Funkin;

class Resource final {
    friend class Funkin;
private:
    Resource();
    ~Resource();

public:
    Resource(Resource const&) = delete;
    Resource(Resource&&) = delete;
    Resource& operator =(Resource const&) = delete;
    Resource& operator =(Resource&&) = delete;

    [[nodiscard]] sptr<Texture> load_shared_texture(std::string_view file);

private:
    string_map<wptr<Texture>> textures_;
};

}