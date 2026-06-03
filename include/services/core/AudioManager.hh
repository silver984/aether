#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>

namespace aether {

class Aether;
class Sound;

class AudioManager final {
	friend class Aether;

private:
	AudioManager();

public:
	struct generation_descriptor final {
		std::shared_ptr<Sound> owner;
		std::string_view file;
	};

	~AudioManager();
	AudioManager(AudioManager const&)            = delete;
	AudioManager(AudioManager&&)                 = delete;
	AudioManager& operator=(AudioManager const&) = delete;
	AudioManager& operator=(AudioManager&&)      = delete;

	[[nodiscard]] std::optional<std::uint32_t> generate_handle(generation_descriptor desc);
	bool play(std::uint32_t id);

private:
	bool init();
	void shutdown();
	void update();
	void pause();
	void resume();
	[[nodiscard]] bool is_paused();

	struct impl;
	std::unique_ptr<impl> impl_;
};

} // namespace aether