set(AE_SAMPLES_DIR "${AE_BIN_DIR}/samples")
option(AE_BUILD_ALL_MODULE_SAMPLES "Build all module samples" OFF)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/log")
target_link_libraries(aether_engine_core PUBLIC aether_engine_log)