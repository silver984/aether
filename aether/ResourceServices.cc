#include <ResourceServices.hh>

namespace aether {

// private
ResourceServices::ResourceServices(TextureRepository& textures, AnimationRepository& animations,
                                   AudioRepository& audios)
    : textures_(textures)
    , animations_(animations)
    , audios_(audios) {}

ResourceServices::~ResourceServices() = default;

TextureRepository& ResourceServices::textures() const {
	return textures_;
}

AnimationRepository& ResourceServices::animations() const {
	return animations_;
}

AudioRepository& ResourceServices::audios() const {
	return audios_;
}

} // namespace aether