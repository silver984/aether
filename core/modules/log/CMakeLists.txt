add_library(ae_log STATIC)

target_include_directories(ae_log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_sources(ae_log PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/log.cc)

target_link_libraries(ae_log PUBLIC fmt::fmt)

option(AE_BUILD_LOG_SAMPLE "Build log sample" OFF)
if (AE_BUILD_ALL_MODULE_SAMPLES OR AE_BUILD_LOG_SAMPLE)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sample)
endif()