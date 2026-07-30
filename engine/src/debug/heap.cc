#include <debug/heap.hh>
#if defined(AE_HAS_DEBUG)
	#include <cstdlib>

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