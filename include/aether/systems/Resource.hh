#pragma once
#include <aether/common/string_map.hh>
#include <memory>
#include <aether/graphics/Texture.hh>
#include <string_view>
#include <cstdint>

namespace ae {

class Funkin;
class Director;

class Resource final {
    friend class Funkin;
    friend class Director;
public:
    Resource();
    ~Resource();
    Resource(Resource const&) = delete;
    Resource(Resource&&) = delete;
    Resource& operator=(Resource const&) = delete;
    Resource& operator=(Resource&&) = delete;

    [[nodiscard]] std::shared_ptr<Texture> load_shared_texture(std::string_view file);

private:
    void clean_refs();
    size_t clean_texture_refs();

    string_map<std::weak_ptr<Texture>> textures_refs_;
};

}