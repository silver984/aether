#pragma once
#include <data/u8vec.hh>
#include <soloud_wav.h>
#include <sref.hh>
#include <string_view>

namespace SoLoud {

class Soloud;

}

namespace aether {

class context;

class sound final {
public:
	sound(context const& ctx);
	~sound();
	[[nodiscard]] static sref<sound> create(context const& ctx, std::string_view file);
	bool play();
	void set_volume(float val);
	[[nodiscard]] float volume() const;
	void set_pan(float val);
	[[nodiscard]] float pan() const;
	void pause();
	[[nodiscard]] bool is_paused();
	void resume();

private:
	bool init_(audio_repository& audios, std::string_view file);
	[[nodiscard]] bool is_wave_handle_valid_();
	[[nodiscard]] int voice_count_();

	SoLoud::Soloud& soloud_;
	SoLoud::Wav wave_;
	SoLoud::handle wave_handle_;
	sref<u8vec> buffer_;
	float volume_;
	float pan_;
};

} // namespace aether