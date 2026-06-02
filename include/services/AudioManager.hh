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
	~AudioManager();
	AudioManager(AudioManager const&)            = delete;
	AudioManager(AudioManager&&)                 = delete;
	AudioManager& operator=(AudioManager const&) = delete;
	AudioManager& operator=(AudioManager&&)      = delete;

	[[nodiscard]] std::optional<std::uint32_t> generate_handle(std::shared_ptr<Sound> owner, std::string_view file);
	bool play(std::uint32_t id);

private:
	bool init();
	void shutdown();
	void update();

	struct impl;
	std::unique_ptr<impl> impl_;
};

} // namespace aether