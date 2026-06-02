#pragma once

namespace aether {

class Context;
class TextureRepository;
class AnimationRepository;

class ResourceServices final {
	friend class Context;

private:
	ResourceServices(TextureRepository& _texture_repository, AnimationRepository& _animation_repository);

public:
	~ResourceServices();
	ResourceServices(ResourceServices const&)            = delete;
	ResourceServices(ResourceServices&&)                 = delete;
	ResourceServices& operator=(ResourceServices const&) = delete;
	ResourceServices& operator=(ResourceServices&&)      = delete;

	TextureRepository& texture_repository;
	AnimationRepository& animation_repository;
};

} // namespace aether