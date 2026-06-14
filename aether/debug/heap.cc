#ifdef AETHER_DEBUG
	#include <cmath>
	#include <cstdlib>
	#include <debug/heap.hh>

namespace {

size_t current      = 0;
size_t last_current = 0;
size_t total        = 0;

} // namespace

void* operator new(size_t size) {
	current += size;
	return std::malloc(size);
}

void* operator new[](size_t size) {
	current += size;
	return std::malloc(size);
}

void operator delete(void* ptr, size_t size) noexcept {
	if (ptr) {
		current -= size;
		std::free(ptr);
	}
}

void operator delete[](void* ptr, size_t size) noexcept {
	if (ptr) {
		current -= size;
		std::free(ptr);
	}
}

namespace aether::heap {

size_t usage() {
	return current;
}

size_t total_usage() {
	if (last_current != current) {
		total        = std::max(total, current);
		last_current = current;
	}

	return total;
}

} // namespace aether::heap
#endif