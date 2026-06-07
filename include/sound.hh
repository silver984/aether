#pragma once
#include <cstdint>
#include <memory>
#include <string_view>

namespace aether {

class context;

class sound final {
public:
	sound(context const& ctx);
	~sound();

	[[nodiscard]] static std::shared_ptr<sound> create(context const& ctx, std::string_view file);
	bool play() const;
	void set_volume(float val) const;
	[[nodiscard]] float volume() const;
	void set_pan(float val) const;
	[[nodiscard]] float pan() const;
	void pause() const;
	bool is_paused() const;
	void resume() const;
	[[nodiscard]] uint32_t voice_count() const;

private:
	struct impl;
	std::unique_ptr<impl> impl_;
};

} // namespace aether