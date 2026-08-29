#pragma once
#include <aether/general.h>
#if defined(__ae_anydebug__)
	#include <cstddef>

namespace aether::heap {

[[nodiscard]] size_t usage();

} // namespace aether::heap
#endif