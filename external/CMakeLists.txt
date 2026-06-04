add_subdirectory(miniaudio)
add_subdirectory(tinyxml2)

# SoLoud

file(GLOB_RECURSE SOLOUD_AUDIOSRC soloud/src/audiosource/*.c*)
file(GLOB_RECURSE SOLOUD_CORE soloud/src/core/*.c*)
file(GLOB_RECURSE SOLOUD_FILTER soloud/src/filter/*.c*)

add_library(SoLoud STATIC
    soloud/src/backend/miniaudio/soloud_miniaudio.cpp
    soloud/src/c_api/soloud_c.cpp
    ${SOLOUD_AUDIOSRC}
    ${SOLOUD_CORE}
    ${SOLOUD_FILTER}
)

target_include_directories(SoLoud
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/soloud/include
)

target_compile_definitions(SoLoud
    PUBLIC
        WITH_MINIAUDIO
)

target_link_libraries(${AETHER} PRIVATE SoLoud)