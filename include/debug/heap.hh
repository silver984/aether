#pragma once
#if (defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB))
	#include <cstddef>

namespace aether::heap {

[[nodiscard]] size_t usage();
[[nodiscard]] size_t total_usage();

} // namespace aether::heap
#endif