file(
  GLOB
  AETHER_ENGINE_NODES_SRC
    nodes/*.cc
)

file(
  GLOB
  AETHER_ENGINE_UTIL_SRC
    util/*.cc
)

file(
  GLOB
  AETHER_ENGINE_GENERAL_SRC
    *.cc
)

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
    $<$<CONFIG:Debug,RelWithDebInfo>:${CMAKE_CURRENT_SOURCE_DIR}/debug/log.cc>
    $<$<CONFIG:Debug,RelWithDebInfo>:${CMAKE_CURRENT_SOURCE_DIR}/debug/heap.cc>
    math/mat3.cc
    ${AETHER_ENGINE_NODES_SRC}
    ${AETHER_ENGINE_UTIL_SRC}
    ${AETHER_ENGINE_GENERAL_SRC}
)