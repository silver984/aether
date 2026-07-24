#pragma once
#include <cache.hh>

namespace aether {

class game;
class texture2d;

class texture_cache final : public cache<texture2d> {
	friend class game;

public:
	~texture_cache();

protected:
	[[nodiscard]] ref<texture2d> load_(fs::path const& file) override;

private:
	texture_cache();
};

} // namespace aether