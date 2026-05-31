#include <aether/objects/abstract/Sound.hh>
#include <aether/systems/Audio.hh>
#include <aether/systems/Context.hh>

namespace ae {

Sound::Sound(Context const& ctx) : ctx_(ctx), id_(0) {}
Sound::~Sound() = default;

std::shared_ptr<Sound> Sound::create(Context const& ctx, std::string_view file) {
	std::shared_ptr<Sound> ptr = std::make_shared<Sound>(ctx);

	if (auto const optional_handle = ctx.audio.generate_handle(ptr, file); optional_handle.has_value()) {
		ptr->id_ = optional_handle.value();
		return ptr;
	}

	return nullptr;
}

bool Sound::play() const {
	return ctx_.audio.play(id_);
}

} // namespace ae