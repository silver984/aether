file(
  GLOB
  AETHER_ENGINE_NODES_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/nodes/*.cc
)

file(
  GLOB
  AETHER_ENGINE_UTIL_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/util/*.cc
)

file(
  GLOB
  AETHER_ENGINE_GENERAL_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cc
)

# temporary
list(
  REMOVE_ITEM
  AETHER_ENGINE_NODES_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/nodes/animated_sprite.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/nodes/tilemap.cc
)

# temporary
list(
  REMOVE_ITEM
  AETHER_ENGINE_GENERAL_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/animation_repository.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_repository.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/camera.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/sound.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_cache.cc
)

target_sources(
  ${AETHER_ENGINE}
  PRIVATE
    $<$<CONFIG:Debug,RelWithDebInfo>:${CMAKE_CURRENT_SOURCE_DIR}/debug/log.cc>
    $<$<CONFIG:Debug,RelWithDebInfo>:${CMAKE_CURRENT_SOURCE_DIR}/debug/heap.cc>
    ${CMAKE_CURRENT_SOURCE_DIR}/math/mat3.cc
    ${AETHER_ENGINE_NODES_SRC}
    ${AETHER_ENGINE_UTIL_SRC}
    ${AETHER_ENGINE_GENERAL_SRC}
)