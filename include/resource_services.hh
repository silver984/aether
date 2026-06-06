#pragma once

namespace aether {

class context;
class texture_repository;
class animation_repository;
class audio_repository;

class resource_services final {
	friend class context;

private:
	resource_services(texture_repository& textures, animation_repository& animations, audio_repository& audios);

public:
	~resource_services();

	[[nodiscard]] texture_repository& textures() const;
	[[nodiscard]] animation_repository& animations() const;
	[[nodiscard]] audio_repository& audios() const;

private:
	texture_repository& textures_;
	animation_repository& animations_;
	audio_repository& audios_;
};

} // namespace aether