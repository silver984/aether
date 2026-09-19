project(aether.engine_log VERSION 1.0.0 LANGUAGES C CXX)
add_library(aether.engine_log STATIC)

file(GLOB AE_LOG_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*)
target_sources(aether.engine_log PRIVATE ${AE_LOG_SRC})
target_include_directories(aether.engine_log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(aether.engine_log PUBLIC fmt::fmt)

option(AE_BUILD_LOG_SAMPLE "Build log sample" OFF)
if (AE_BUILD_ALL_MODULE_SAMPLES OR AE_BUILD_LOG_SAMPLE)
  add_subdirectory(sample)
endif()