file(GLOB_RECURSE AETHER_LUA_BINDINGS_SRC bindings/*.cc)
target_sources(${AETHER} PRIVATE binding.cc ${AETHER_LUA_BINDINGS_SRC})