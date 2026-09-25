add_library(log STATIC)
add_library(aether-engine::log ALIAS log)

target_include_directories(log PUBLIC include)
target_sources(log PRIVATE src/log.cc)
target_link_libraries(log PUBLIC fmt::fmt)

option(AETHER_BUILD_LOG_SAMPLE "Build log sample" OFF)
if (AETHER_BUILD_ALL_MODULE_SAMPLES OR AETHER_BUILD_LOG_SAMPLE)
  add_subdirectory(sample)
endif()