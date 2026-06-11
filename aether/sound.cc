#include <algorithm>
#include <audio_repository.hh>
#include <context.hh>
#include <debug/log.hh>
#include <soloud_error.h>
#include <sound.hh>

namespace aether {

sound::sound(context const& ctx)
        : soloud_(ctx.core().soloud())
        , wave_handle_(0)
        , volume_(1.f)
        , pan_(0.f) {}

sound::~sound() = default;

sref<sound> sound::create(context const& ctx, std::string_view file) {
	sref<sound> ptr = new sound(ctx);
	if (!ptr->init_(ctx.resource().audios(), file)) {
		return nullptr;
	}
	return ptr;
}

bool sound::play() {
	wave_handle_ = soloud_.play(wave_, volume_, pan_);
	return wave_handle_ != 0;
}

void sound::set_volume(float val) {
	if (volume_ == val) {
		return;
	}

	volume_ = std::clamp(val, 0.f, 1.f);

	if (is_wave_handle_valid_()) {
		soloud_.setVolume(wave_handle_, volume_);
	}
}

float sound::volume() const {
	return volume_;
}

void sound::set_pan(float val) {
	if (pan_ == val) {
		return;
	}

	pan_ = std::clamp(val, -1.f, 1.f);

	if (is_wave_handle_valid_()) {
		soloud_.setPan(wave_handle_, pan_);
	}
}

float sound::pan() const {
	return pan_;
}

void sound::pause() {
	if (is_wave_handle_valid_()) {
		soloud_.setPause(wave_handle_, true);
	}
}

bool sound::is_paused() {
	if (!is_wave_handle_valid_()) {
		return false;
	}

	return soloud_.getPause(wave_handle_);
}

void sound::resume() {
	if (is_wave_handle_valid_()) {
		soloud_.setPause(wave_handle_, false);
	}
}

bool sound::init_(audio_repository& audios, std::string_view file) {
	byte_buffer_ = audios.fetch(file);

	if (!byte_buffer_) {
		return false;
	}

	using enum SoLoud::SOLOUD_ERRORS;
	if (wave_.loadMem(byte_buffer_->data(), byte_buffer_->size(), false, false) != SO_NO_ERROR) {
		return false;
	}

	return true;
}

bool sound::is_wave_handle_valid_() {
	if (wave_handle_ == 0) {
		return false;
	}

	bool const out = soloud_.isValidVoiceHandle(wave_handle_);

	if (!out) {
		wave_handle_ = 0;
	}

	return out;
}

int sound::voice_count_() {
	return soloud_.countAudioSource(wave_);
}

} // namespace aether