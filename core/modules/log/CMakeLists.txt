add_library(aether_engine_log STATIC)

set(AE_LOG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE AE_EXCLUDED_LOG_HEADERS CONFIGURE_DEPENDS ${AE_LOG_INCLUDE_DIR}/aether/log-impl/*.h*)
list(REMOVE_ITEM AE_LOG_INCLUDE_DIR ${AE_EXCLUDED_LOG_HEADERS})

target_include_directories(aether_engine_log PUBLIC ${AE_LOG_INCLUDE_DIR})
target_sources(aether_engine_log PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/log.cc)

target_link_libraries(aether_engine_log PUBLIC fmt::fmt)

option(AE_BUILD_LOG_SAMPLE "Build log sample" OFF)
if (AE_BUILD_ALL_MODULE_SAMPLES OR AE_BUILD_LOG_SAMPLE)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sample)
endif()