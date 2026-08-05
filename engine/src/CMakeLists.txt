file(GLOB AETHER_ENGINE_MATH_SRC math/*.c*)
file(GLOB AETHER_ENGINE_NODES_SRC nodes/*.c*)
file(GLOB AETHER_ENGINE_UTIL_SRC util/*.c*)
file(GLOB AETHER_ENGINE_GENERAL_SRC *.c*)

# temporary
list(
  REMOVE_ITEM
  AETHER_ENGINE_NODES_SRC
    nodes/animated_sprite.cc
    nodes/tilemap.cc
)

# temporary
list(
  REMOVE_ITEM
  AETHER_ENGINE_GENERAL_SRC
    animation_repository.cc
    audio_repository.cc
    camera.cc
    sound.cc
    texture_cache.cc
)

target_sources(
  ${AETHER_ENGINE}
  PRIVATE
    ${AETHER_ENGINE_MATH_SRC}
    ${AETHER_ENGINE_NODES_SRC}
    ${AETHER_ENGINE_UTIL_SRC}
    ${AETHER_ENGINE_GENERAL_SRC}
)

if(CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
  file(GLOB AETHER_ENGINE_DEBUG_SRC debug/*.c*)
  target_sources(${AETHER_ENGINE} PRIVATE ${AETHER_ENGINE_DEBUG_SRC})
endif()