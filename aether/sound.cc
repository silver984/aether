#include <algorithm>
#include <context.hh>
#ifdef AETHER_DEBUG
	#include <debug/log.hh>
#endif
#include <audio_repository.hh>
#include <soloud.h>
#include <soloud_error.h>
#include <soloud_wav.h>
#include <sound.hh>

using enum SoLoud::SOLOUD_ERRORS;

namespace aether {

struct sound::impl final {
	impl(context const& ctx)
	        : soloud_(ctx.core().soloud())
	        , wave_handle_(0)
	        , volume_(1.f)
	        , pan_(0.f) {}

	bool init(audio_repository& audios, std::string_view file) {
		buffer_ = audios.fetch(file);

		if (!buffer_) {
			return false;
		}

		if (wave_.loadMem(buffer_->data(), buffer_->size(), false, false) != SO_NO_ERROR) {
			return false;
		}

		return true;
	}

	bool play() {
		wave_handle_ = soloud_.play(wave_, volume_, pan_);
		return wave_handle_ != 0;
	}

	void set_volume(float val) {
		if (volume_ == val) {
			return;
		}

		volume_ = std::clamp(val, 0.f, 1.f);

		if (is_wave_handle_valid()) {
			soloud_.setVolume(wave_handle_, volume_);
		}
	}

	void set_pan(float val) {
		if (pan_ == val) {
			return;
		}

		pan_ = std::clamp(val, -1.f, 1.f);

		if (is_wave_handle_valid()) {
			soloud_.setPan(wave_handle_, pan_);
		}
	}

	void pause() {
		if (is_wave_handle_valid()) {
			soloud_.setPause(wave_handle_, true);
		}
	}

	[[nodiscard]] bool is_paused() {
		if (!is_wave_handle_valid()) {
			return false;
		}

		return soloud_.getPause(wave_handle_);
	}

	void resume() {
		if (is_wave_handle_valid()) {
			soloud_.setPause(wave_handle_, false);
		}
	}

	[[nodiscard]] uint32_t voice_count() {
		return (uint32_t)soloud_.countAudioSource(wave_);
	}

	[[nodiscard]] bool is_wave_handle_valid() {
		if (wave_handle_ == 0) {
			return false;
		}

		bool const out = soloud_.isValidVoiceHandle(wave_handle_);

		if (!out) {
			wave_handle_ = 0;
		}

		return out;
	}

	SoLoud::Soloud& soloud_;
	SoLoud::Wav wave_;
	SoLoud::handle wave_handle_;
	std::shared_ptr<byte_buffer> buffer_;
	float volume_;
	float pan_;
};

sound::sound(context const& ctx)
        : impl_(std::make_unique<impl>(ctx)) {}

sound::~sound() = default;

std::shared_ptr<sound> sound::create(context const& ctx, std::string_view file) {
	std::shared_ptr<sound> ptr = std::make_shared<sound>(ctx);

	if (!ptr->impl_->init(ctx.resource().audios(), file)) {
		return nullptr;
	}

	return ptr;
}

bool sound::play() const {
	return impl_->play();
}

void sound::set_volume(float val) const {
	return impl_->set_volume(val);
}

float sound::volume() const {
	return impl_->volume_;
}

void sound::set_pan(float val) const {
	return impl_->set_pan(val);
}

float sound::pan() const {
	return impl_->pan_;
}

void sound::pause() const {
	impl_->pause();
}

bool sound::is_paused() const {
	return impl_->is_paused();
}

void sound::resume() const {
	impl_->resume();
}

uint32_t sound::voice_count() const {
	return impl_->voice_count();
}

} // namespace aether