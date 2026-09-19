add_library(aether_engine_log STATIC)

set(AE_LOG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(GLOB_RECURSE AE_LOG_HEADERS CONFIGURE_DEPENDS "${AE_LOG_INCLUDE_DIR}/aether/*.h*")
list(FILTER AE_LOG_HEADERS EXCLUDE REGEX "^${AE_LOG_INCLUDE_DIR}/aether/log_impl/")

file(GLOB AE_LOG_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*")

target_sources(
  aether_engine_log
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${AE_LOG_INCLUDE_DIR}
    FILES ${AE_LOG_HEADERS}
  PRIVATE ${AE_LOG_SRC}
)

target_link_libraries(aether_engine_log PUBLIC fmt::fmt)

option(AE_BUILD_LOG_SAMPLE "Build log sample" OFF)
if (AE_BUILD_ALL_MODULE_SAMPLES OR AE_BUILD_LOG_SAMPLE)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/sample")
endif()