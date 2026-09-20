target_sources(
  aether_engine
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/miniz/miniz.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2/tinyxml2.cpp
)

add_library(soloud STATIC)

set(SOLOUD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/soloud/include)
set(SOLOUD_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/soloud/src)

file(GLOB SOLOUD_HEADERS CONFIGURE_DEPENDS ${SOLOUD_INCLUDE_DIR}/*.h*)

file(GLOB_RECURSE SOLOUD_AUDIO_SRC ${SOLOUD_SRC_DIR}/audiosource/*.c*)
file(GLOB_RECURSE SOLOUD_CORE_SRC ${SOLOUD_SRC_DIR}/core/*.c*)
file(GLOB_RECURSE SOLOUD_FILTER_SRC ${SOLOUD_SRC_DIR}/filter/*.c*)

target_compile_definitions(soloud PRIVATE WITH_MINIAUDIO)

target_sources(
  soloud
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${SOLOUD_INCLUDE_DIR}
    FILES ${SOLOUD_HEADERS}
  PRIVATE
    ${SOLOUD_SRC_DIR}/backend/miniaudio/soloud_miniaudio.cpp
    ${SOLOUD_SRC_DIR}/c_api/soloud_c.cpp
    ${SOLOUD_AUDIO_SRC}
    ${SOLOUD_CORE_SRC}
    ${SOLOUD_FILTER_SRC}
)