#include <resource_services.hh>

namespace aether {

// private
resource_services::resource_services(texture_repository& textures, animation_repository& animations,
                                     audio_repository& audios)
    : textures_(textures)
    , animations_(animations)
    , audios_(audios) {}

resource_services::~resource_services() = default;

texture_repository& resource_services::textures() const {
	return textures_;
}

animation_repository& resource_services::animations() const {
	return animations_;
}

audio_repository& resource_services::audios() const {
	return audios_;
}

} // namespace aether