#pragma once

#include <cstdint>

namespace aether::_ref_impl {

struct shared_block_ final {
	void* ptr;
	void (*releaser)(void*);
	uint32_t strong_count;
	uint32_t weak_count;
};

struct unique_block_ final {
	void* ptr;
	void (*releaser)(void*);
};

} // namespace aether::_ref_impl