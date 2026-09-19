target_sources(
  aether.engine PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/miniz/miniz.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2/tinyxml2.cpp
)

project(soloud VERSION 1.0.0 LANGUAGES C CXX)
add_library(soloud STATIC)

set(SOLOUD_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/soloud/src)
file(GLOB_RECURSE SOLOUD_AUDIO_SRC ${SOLOUD_SRC_DIR}/audiosource/*.c*)
file(GLOB_RECURSE SOLOUD_CORE_SRC ${SOLOUD_SRC_DIR}/core/*.c*)
file(GLOB_RECURSE SOLOUD_FILTER_SRC ${SOLOUD_SRC_DIR}/filter/*.c*)

target_sources(
  soloud PRIVATE
  ${SOLOUD_SRC_DIR}/backend/miniaudio/soloud_miniaudio.cpp
  ${SOLOUD_SRC_DIR}/c_api/soloud_c.cpp
  ${SOLOUD_AUDIO_SRC}
  ${SOLOUD_CORE_SRC}
  ${SOLOUD_FILTER_SRC}
)

target_include_directories(soloud PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/soloud/include)
target_compile_definitions(soloud PRIVATE WITH_MINIAUDIO)
target_link_libraries(aether.engine PRIVATE soloud)