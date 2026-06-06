#pragma once

namespace aether {

class Context;
class TextureRepository;
class AnimationRepository;
class AudioRepository;

class ResourceServices final {
	friend class Context;

private:
	ResourceServices(TextureRepository& textures, AnimationRepository& animations, AudioRepository& audios);

public:
	~ResourceServices();

	[[nodiscard]] TextureRepository& textures() const;
	[[nodiscard]] AnimationRepository& animations() const;
	[[nodiscard]] AudioRepository& audios() const;

private:
	TextureRepository& textures_;
	AnimationRepository& animations_;
	AudioRepository& audios_;
};

} // namespace aether