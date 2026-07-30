#pragma once
#include <config.h>
#if defined(AE_HAS_DEBUG)
	#include <cstddef>

namespace aether::heap {

[[nodiscard]] size_t usage();

} // namespace aether::heap
#endif