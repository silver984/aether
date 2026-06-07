# tinyxml2
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2/tinyxml2.cpp)

# SoLoud
file(GLOB_RECURSE SOLOUD_AUDIO_SRC ${CMAKE_CURRENT_SOURCE_DIR}/soloud/src/audiosource/*.c*)
file(GLOB_RECURSE SOLOUD_CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/soloud/src/core/*.c*)
file(GLOB_RECURSE SOLOUD_FILTER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/soloud/src/filter/*.c*)

add_library(SoLoud STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/soloud/src/backend/miniaudio/soloud_miniaudio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/soloud/src/c_api/soloud_c.cpp
    ${SOLOUD_AUDIO_SRC} ${SOLOUD_CORE_SRC} ${SOLOUD_FILTER_SRC}
)

target_include_directories(SoLoud PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/soloud/include)
target_compile_definitions(SoLoud PRIVATE WITH_MINIAUDIO)
target_link_libraries(${PROJECT_NAME} PRIVATE SoLoud)