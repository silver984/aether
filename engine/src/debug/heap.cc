#if (defined(AETHER_DEBUG) || defined(AETHER_RELWITHDEB))
	#include <cstdlib>
	#include <debug/heap.hh>

namespace {

size_t current = 0;

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

} // namespace aether::heap
#endif