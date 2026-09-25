target_include_directories(app PRIVATE miniz)
target_sources(app PRIVATE miniz/miniz.c)

add_library(soloud STATIC)

file(GLOB_RECURSE SOLOUD_AUDIO_SOURCE soloud/src/audiosource/*.c*)
file(GLOB_RECURSE SOLOUD_CORE_SOURCE soloud/src/core/*.c*)
file(GLOB_RECURSE SOLOUD_FILTER_SOURCE soloud/src/filter/*.c*)

target_compile_definitions(soloud PRIVATE WITH_MINIAUDIO)
target_include_directories(soloud PUBLIC soloud/include)

target_sources(
  soloud PRIVATE
  soloud/src/backend/miniaudio/soloud_miniaudio.cpp
  soloud/src/c_api/soloud_c.cpp
  ${SOLOUD_AUDIO_SOURCE}
  ${SOLOUD_CORE_SOURCE}
  ${SOLOUD_FILTER_SOURCE}
)

target_link_libraries(app PUBLIC soloud)