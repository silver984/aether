#include <ResourceServices.hh>

namespace aether {

// private
ResourceServices::ResourceServices(TextureRepository& _texture_repository, AnimationRepository& _animation_repository,
                                   AudioRepository& _audio_repository)
    : texture_repository(_texture_repository)
    , animation_repository(_animation_repository)
    , audio_repository(_audio_repository) {}

ResourceServices::~ResourceServices() = default;

} // namespace aether