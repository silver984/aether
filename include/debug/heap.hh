#pragma once
#ifdef AETHER_DEBUG
	#include <cstdint>

namespace aether::heap {

[[nodiscard]] size_t usage();

}
#endif