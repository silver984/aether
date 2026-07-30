file(
  GLOB
  AE_NODES_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/nodes/*.cc
)

file(
  GLOB
  AE_UTIL_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/util/*.cc
)

file(
  GLOB
  AE_GENERAL_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cc
)

# temporary
list(
  REMOVE_ITEM
  AE_NODES_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/nodes/animated_sprite.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/nodes/tilemap.cc
)

# temporary
list(
  REMOVE_ITEM
  AE_GENERAL_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/animation_repository.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_repository.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/camera.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/sound.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_cache.cc
)

target_sources(
  ${AE}
  PRIVATE
    $<$<CONFIG:Debug,RelWithDebInfo>:${CMAKE_CURRENT_SOURCE_DIR}/debug/log.cc>
    $<$<CONFIG:Debug,RelWithDebInfo>:${CMAKE_CURRENT_SOURCE_DIR}/debug/heap.cc>
    ${CMAKE_CURRENT_SOURCE_DIR}/math/mat3.cc
    ${AE_NODES_SRC}
    ${AE_UTIL_SRC}
    ${AE_GENERAL_SRC}
)