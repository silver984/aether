#include <Context.hh>
#include <algorithm>
#include <log.hh>
#include <objects/abstract/Sound.hh>
#include <services/resource/AudioRepository.hh>
#include <soloud.h>
#include <soloud_error.h>
#include <soloud_wav.h>

using enum SoLoud::SOLOUD_ERRORS;

namespace aether {

struct Sound::impl final {
	impl(Context const& ctx)
	    : soloud(ctx.core_services.soloud)
	    , wave_handle(0)
	    , volume(1.f)
	    , pan(0.f) {}

	bool init(Context const& ctx, std::string_view file) {
		buffer = ctx.resource_services.audio_repository.fetch(file);

		if (!buffer) {
			return false;
		}

		if (wave.loadMem(buffer->data(), buffer->size(), false, false) != SO_NO_ERROR) {
			return false;
		}

		return true;
	}

	bool play() {
		wave_handle = soloud.play(wave, volume, pan);
		return wave_handle != 0;
	}

	SoLoud::Soloud& soloud;
	SoLoud::Wav wave;
	std::shared_ptr<byte_buffer> buffer;
	unsigned int wave_handle;
	float volume;
	float pan;
};

Sound::Sound(Context const& ctx)
    : impl_(std::make_unique<impl>(ctx)) {}

Sound::~Sound() = default;

std::shared_ptr<Sound> Sound::create(Context const& ctx, std::string_view file) {
	std::shared_ptr<Sound> ptr = std::make_shared<Sound>(ctx);

	if (!ptr->init(ctx, file)) {
		return nullptr;
	}

	return ptr;
}

// private
bool Sound::init(Context const& ctx, std::string_view file) {
	return impl_->init(ctx, file);
}

bool Sound::play() {
	return impl_->play();
}

} // namespace aether