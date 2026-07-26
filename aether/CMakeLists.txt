file(GLOB AETHER_LUA_BINDINGS_SRC lua/bindings/*.cc)
file(GLOB AETHER_NODES_SRC nodes/*.cc)
file(GLOB AETHER_UTIL_SRC util/*.cc)
file(GLOB AETHER_GENERAL_SRC *.cc)

list(
    REMOVE_ITEM
    AETHER_NODES_SRC
        animated_sprite.cc
        tilemap.cc
)

list(
    REMOVE_ITEM
    AETHER_GENERAL_SRC
        animation_repository.cc
        audio_repository.cc
        camera.cc
        sound.cc
        texture_cache.cc
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug,RelWithDebInfo>:${CMAKE_CURRENT_SOURCE_DIR}/debug/log.cc>
        $<$<CONFIG:Debug,RelWithDebInfo>:${CMAKE_CURRENT_SOURCE_DIR}/debug/heap.cc>
        lua/manager.cc
        math/mat3.cc
        ${AETHER_LUA_BINDINGS_SRC}
        ${AETHER_NODES_SRC}
        ${AETHER_UTIL_SRC}
        ${AETHER_GENERAL_SRC}
)