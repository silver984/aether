#include <Context.hh>
#include <objects/abstract/Sound.hh>
#include <services/AudioManager.hh>

namespace aether {

Sound::Sound(Context const& ctx)
    : ctx_(ctx)
    , id_(0) {}

Sound::~Sound() = default;

std::shared_ptr<Sound> Sound::create(Context const& ctx, std::string_view file) {
	std::shared_ptr<Sound> ptr = std::make_shared<Sound>(ctx);

	if (auto const optional_handle = ctx.audio_manager.generate_handle({.owner = ptr, .file = file});
	    optional_handle.has_value()) {
		ptr->id_ = optional_handle.value();
		return ptr;
	}

	return nullptr;
}

bool Sound::play() const {
	return ctx_.audio_manager.play(id_);
}

} // namespace aether