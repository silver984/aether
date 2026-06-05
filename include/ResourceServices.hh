#pragma once

namespace aether {

class Context;
class TextureRepository;
class AnimationRepository;
class AudioRepository;

class ResourceServices final {
	friend class Context;

private:
	ResourceServices(TextureRepository& _texture_repository, AnimationRepository& _animation_repository,
	                 AudioRepository& _audio_repository);

public:
	~ResourceServices();
	ResourceServices(ResourceServices const&)            = delete;
	ResourceServices(ResourceServices&&)                 = delete;
	ResourceServices& operator=(ResourceServices const&) = delete;
	ResourceServices& operator=(ResourceServices&&)      = delete;

	TextureRepository& texture_repository;
	AnimationRepository& animation_repository;
	AudioRepository& audio_repository;
};

} // namespace aether