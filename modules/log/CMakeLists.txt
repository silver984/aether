add_library(aether.engine-log STATIC)

target_include_directories(aether.engine-log PUBLIC include)
target_sources(aether.engine-log PRIVATE src/log.cc)
target_link_libraries(aether.engine-log PUBLIC fmt::fmt)

option(AETHER_BUILD_LOG_SAMPLE "Build log sample" OFF)
if (AETHER_BUILD_ALL_MODULE_SAMPLES OR AETHER_BUILD_LOG_SAMPLE)
  add_subdirectory(sample)
endif()