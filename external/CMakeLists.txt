# tinyxml2
target_sources(
    ${PROJECT_NAME}
    PRIVATE
        tinyxml2/tinyxml2.cpp
)

# miniz
target_sources(
    ${PROJECT_NAME}
    PRIVATE
        miniz/miniz.c
)

# SoLoud
file(GLOB_RECURSE SOLOUD_AUDIO_SRC soloud/src/audiosource/*.c*)
file(GLOB_RECURSE SOLOUD_CORE_SRC soloud/src/core/*.c*)
file(GLOB_RECURSE SOLOUD_FILTER_SRC soloud/src/filter/*.c*)

add_library(
    SoLoud
    STATIC
        soloud/src/backend/miniaudio/soloud_miniaudio.cpp
        soloud/src/c_api/soloud_c.cpp
        ${SOLOUD_AUDIO_SRC}
        ${SOLOUD_CORE_SRC}
        ${SOLOUD_FILTER_SRC}
)

target_include_directories(
    SoLoud
    PUBLIC
        soloud/include
)

target_compile_definitions(
    SoLoud
    PRIVATE
        WITH_MINIAUDIO
)

target_link_libraries(${PROJECT_NAME} PRIVATE SoLoud)