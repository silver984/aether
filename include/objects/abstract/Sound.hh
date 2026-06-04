#pragma once
#include <cstdint>
#include <memory>
#include <string_view>

namespace aether {

class Context;

class Sound final {
public:
	Sound(Context const& ctx);
	~Sound();

	static std::shared_ptr<Sound> create(Context const& ctx, std::string_view file);
	[[nodiscard]] std::uint32_t id() const;
	bool play() const;
	bool pause() const;
	[[nodiscard]] float duration() const;
	[[nodiscard]] float time() const;
	[[nodiscard]] bool is_playing() const;
	[[nodiscard]] bool is_finished() const;
	bool seek_time(float seconds) const;

private:
	Context const& ctx_;
	std::uint32_t id_;
};

} // namespace aether