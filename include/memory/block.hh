#pragma once
#include <cstddef>

namespace aether {

template <typename T>
struct block final {
	T* ptr;
	size_t strong_count;
	size_t weak_count;
};

} // namespace aether