#ifdef AETHER_DEBUG
	#include <cstdlib>
	#include <debug/heap.hh>

namespace {

size_t current_usage = 0;

}

void* operator new(size_t size) {
	current_usage += size;
	return std::malloc(size);
}

void operator delete(void* ptr, size_t size) noexcept {
	if (ptr) {
		current_usage -= size;
		std::free(ptr);
	}
}

namespace aether::heap {

size_t usage() {
	return current_usage;
}

} // namespace aether::heap
#endif