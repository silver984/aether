file(
  GLOB_RECURSE
  SOLOUD_AUDIO_SRC
    src/audiosource/*.c*
)

file(
  GLOB_RECURSE
  SOLOUD_CORE_SRC
    src/core/*.c*
)

file(
  GLOB_RECURSE
  SOLOUD_FILTER_SRC
    src/filter/*.c*
)

add_library(
  SoLoud
  STATIC
    src/backend/miniaudio/soloud_miniaudio.cpp
    src/c_api/soloud_c.cpp
    ${SOLOUD_AUDIO_SRC}
    ${SOLOUD_CORE_SRC}
    ${SOLOUD_FILTER_SRC}
)

target_include_directories(
  SoLoud
  PUBLIC
    include
)

target_compile_definitions(
  SoLoud
  PRIVATE
    WITH_MINIAUDIO
)

target_link_libraries(
  ${AETHER_ENGINE}
  PRIVATE
    SoLoud
)