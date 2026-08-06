#pragma once
#include <aether/general.h>
#if defined(AETHER_ENGINE_HAS_DEBUG)
	#include <cstddef>

namespace aether::heap {

[[nodiscard]] size_t usage();

} // namespace aether::heap
#endif