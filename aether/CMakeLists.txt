file(GLOB AETHER_LUA_BINDINGS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/lua/bindings/*.cc)
file(GLOB AETHER_NODES_SRC ${CMAKE_CURRENT_SOURCE_DIR}/nodes/*.cc)
file(GLOB AETHER_UTIL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/util/*.cc)
file(GLOB AETHER_GENERAL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

list(
  REMOVE_ITEM
  AETHER_NODES_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/nodes/animated_sprite.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/nodes/tilemap.cc
)

list(
  REMOVE_ITEM
  AETHER_GENERAL_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/animation_repository.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_repository.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/camera.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/sound.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_cache.cc
)

target_sources(
  ${PROJECT_NAME}
  PRIVATE
    $<$<CONFIG:Debug,RelWithDebInfo>:${CMAKE_CURRENT_SOURCE_DIR}/debug/log.cc>
    $<$<CONFIG:Debug,RelWithDebInfo>:${CMAKE_CURRENT_SOURCE_DIR}/debug/heap.cc>
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/manager.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/math/mat3.cc
    # ${AETHER_LUA_BINDINGS_SRC}
    ${AETHER_NODES_SRC}
    ${AETHER_UTIL_SRC}
    ${AETHER_GENERAL_SRC}
)