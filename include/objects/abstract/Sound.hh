#pragma once
#include <memory>
#include <string_view>

namespace aether {

class Context;

class Sound final {
public:
	Sound(Context const& ctx);
	~Sound();

	[[nodiscard]] static std::shared_ptr<Sound> create(Context const& ctx, std::string_view file);
	bool play();
	void set_volume(float val);
	[[nodiscard]] float volume() const;
	void set_pan(float val);
	[[nodiscard]] float pan() const;

private:
	bool init(Context const& ctx, std::string_view file);
	// [[nodiscard]] bool is_wave_handle_valid();

	struct impl;
	std::unique_ptr<impl> impl_;

	// SoLoud::Soloud& soloud_;
	// std::shared_ptr<byte_buffer> buffer_;
	// unsigned int wave_handle_;
	// float volume_;
	// float pan_;
};

} // namespace aether