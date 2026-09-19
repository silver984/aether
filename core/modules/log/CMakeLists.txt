add_library(aether_engine_log STATIC)

file(GLOB_RECURSE AE_LOG_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/aether/*.h*)
file(GLOB AE_LOG_IMPL_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/aether/log_impl/*.h*)
list(REMOVE_ITEM AE_LOG_HEADERS ${AE_LOG_IMPL_HEADERS})
file(GLOB AE_LOG_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*)

target_sources(
  aether_engine_log
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES ${AE_LOG_HEADERS}
  PRIVATE ${AE_LOG_SRC}
)

target_link_libraries(aether_engine_log PUBLIC fmt::fmt)

option(AE_BUILD_LOG_SAMPLE "Build log sample" OFF)
if (AE_BUILD_ALL_MODULE_SAMPLES OR AE_BUILD_LOG_SAMPLE)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sample)
endif()