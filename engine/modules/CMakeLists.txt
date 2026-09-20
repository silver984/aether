set(AETHER_SAMPLES_DIR ${AETHER_BINARY_DIR}/samples)
option(AETHER_BUILD_ALL_MODULE_SAMPLES "Build all module samples" OFF)
add_subdirectory(log)
target_link_libraries(aether.engine PUBLIC aether.engine-log)