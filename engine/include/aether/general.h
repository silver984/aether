#pragma once

#if defined(AETHER_ENGINE_DEBUG) || defined(AETHER_ENGINE_RELWITHDEB)
	#define AETHER_ENGINE_HAS_DEBUG 1
#endif

#define AETHER_ENGINE_DELETE_COPY_AND_MOVE(__TYPE__)                                                                                       \
	__TYPE__(__TYPE__&&)                 = delete;                                                                                     \
	__TYPE__(__TYPE__ const&)            = delete;                                                                                     \
	__TYPE__& operator=(__TYPE__ const&) = delete;                                                                                     \
	__TYPE__& operator=(__TYPE__&&)      = delete