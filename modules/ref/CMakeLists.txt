add_library(ref INTERFACE)
add_library(aether-engine::ref ALIAS ref)

target_include_directories(ref INTERFACE include)

option(AETHER_BUILD_REF_SAMPLE "Build ref sample" OFF)
if (AETHER_BUILD_ALL_MODULE_SAMPLES OR AETHER_BUILD_REF_SAMPLE)
  add_subdirectory(sample)
endif()