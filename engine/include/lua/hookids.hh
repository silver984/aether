#pragma once
#include <cstdint>

namespace aether::lua {

enum class hookids : uint32_t {
	testscene_init_,
	testscene_update_
};

} // namespace aether::lua