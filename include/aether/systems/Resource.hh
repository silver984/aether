#pragma once
#include <aether/common/string_map.hh>
#include <memory>
#include <aether/graphics/Texture.hh>
#include <string_view>

namespace ae {

class Funkin;

class Resource final {
    friend class Funkin;
public:
    Resource();
    ~Resource();
    Resource(Resource const&) = delete;
    Resource(Resource&&) = delete;
    Resource& operator=(Resource const&) = delete;
    Resource& operator=(Resource&&) = delete;

    [[nodiscard]] std::shared_ptr<Texture> load_shared_texture(std::string_view file);

private:
    string_map<std::weak_ptr<Texture>> textures_;
};

}