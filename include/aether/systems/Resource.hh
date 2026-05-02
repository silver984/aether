#pragma once
#include <aether/common/string_map.hh>
#include <aether/common/pointers.hh>
#include <aether/graphics/texture.hh>
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

    [[nodiscard]] sptr<texture> load_texture(std::string_view file_path);

private:
    string_map<sptr<texture>> textures_;
};

}