#include <aether/systems/repos/TextureRepo.hh>

namespace ae {

TextureRepo::TextureRepo()  = default;
TextureRepo::~TextureRepo() = default;

std::shared_ptr<Texture> fetch(std::string_view file) {}

} // namespace ae