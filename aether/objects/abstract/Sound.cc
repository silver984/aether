#include <Context.hh>
#include <objects/abstract/Sound.hh>
#include <services/core/AudioManager.hh>

namespace aether {

Sound::Sound(Context const& ctx)
    : ctx_(ctx)
    , id_(0) {}

Sound::~Sound() = default;

std::shared_ptr<Sound> Sound::create(Context const& ctx, std::string_view file) {
	std::shared_ptr<Sound> ptr = std::make_shared<Sound>(ctx);

	if (auto const optional_handle = ctx.core_services.audio_manager.generate_handle({.owner = ptr, .file = file});
	    optional_handle.has_value()) {
		ptr->id_ = optional_handle.value();
		return ptr;
	}

	return nullptr;
}

bool Sound::play() const {
	return ctx_.core_services.audio_manager.play(id_);
}

bool Sound::pause() const {
	return ctx_.core_services.audio_manager.pause(id_);
}

float Sound::duration() const {
	return ctx_.core_services.audio_manager.duration(id_);
}

float Sound::time() const {
	return ctx_.core_services.audio_manager.time(id_);
}

bool Sound::is_playing() const {
	return ctx_.core_services.audio_manager.is_playing(id_);
}

bool Sound::seek_time(float seconds) const {
	return ctx_.core_services.audio_manager.seek_time(id_, seconds);
}

bool Sound::is_finished() const {
	return ctx_.core_services.audio_manager.is_finished(id_);
}

} // namespace aether