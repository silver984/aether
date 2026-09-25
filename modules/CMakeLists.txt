set(AETHER_SAMPLES_DIR ${AETHER_BINARY_DIR}/samples)
option(AETHER_BUILD_ALL_MODULE_SAMPLES "Build all module samples" OFF)

add_subdirectory(log)
add_subdirectory(ref)

target_link_libraries(
  app PRIVATE
  aether-engine::log
  aether-engine::ref
)