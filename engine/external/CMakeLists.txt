# miniz
target_sources(${AETHER_ENGINE} PRIVATE miniz/miniz.c)

# soloud

file(GLOB_RECURSE SOLOUD_AUDIO_SRC soloud/src/audiosource/*.c*)
file(GLOB_RECURSE SOLOUD_CORE_SRC soloud/src/core/*.c*)
file(GLOB_RECURSE SOLOUD_FILTER_SRC soloud/src/filter/*.c*)

add_library(
  soloud
  STATIC
    soloud/src/backend/miniaudio/soloud_miniaudio.cpp
    soloud/src/c_api/soloud_c.cpp
    ${SOLOUD_AUDIO_SRC}
    ${SOLOUD_CORE_SRC}
    ${SOLOUD_FILTER_SRC}
)

target_include_directories(soloud PUBLIC soloud/include)
target_compile_definitions(soloud PRIVATE WITH_MINIAUDIO)
target_link_libraries(${AETHER_ENGINE} PRIVATE soloud)

# tinyxml2
target_sources(${AETHER_ENGINE} PRIVATE tinyxml2/tinyxml2.cpp)