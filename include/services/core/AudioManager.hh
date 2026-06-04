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

	[[nodiscard]] std::optional<std::uint32_t> generate_handle(generation_descriptor desc) const;
	bool play(std::uint32_t id) const;
	bool pause(std::uint32_t id) const;
	[[nodiscard]] float duration(std::uint32_t id) const;
	[[nodiscard]] float time(std::uint32_t id) const;
	[[nodiscard]] bool is_playing(std::uint32_t id) const;
	[[nodiscard]] bool is_finished(std::uint32_t id) const;
	bool seek_time(std::uint32_t id, float seconds) const;

private:
	bool init() const;
	void shutdown() const;
	void update() const;
	void pause_device() const;
	void resume_device() const;
	[[nodiscard]] bool is_device_paused() const;

	struct impl;
	std::unique_ptr<impl> impl_;
};

} // namespace aether