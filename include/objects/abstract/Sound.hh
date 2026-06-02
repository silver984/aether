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
	bool play() const;

private:
	Context const& ctx_;
	std::uint32_t id_;
};

} // namespace aether