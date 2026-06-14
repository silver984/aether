#pragma once
#ifdef AETHER_DEBUG
	#include <cstdint>

namespace aether::heap {

[[nodiscard]] size_t usage();
[[nodiscard]] size_t total_usage();

} // namespace aether::heap
#endif