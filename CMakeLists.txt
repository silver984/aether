# AetherEngine

cmake_minimum_required(VERSION 3.31.6)

set(AETHER AetherEngine)
include(cmake/version.cmake)
project(${AETHER} VERSION ${AETHER_VERSION} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(AETHER_VERBOSE_DEBUG "Enable verbose debug" OFF)

add_executable(${AETHER})

# source files
add_subdirectory(aether)
add_subdirectory(external)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/version.h
    @ONLY
)

# includes
target_include_directories(${AETHER}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external
        ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_compile_definitions(${AETHER}
    PRIVATE
        $<$<CONFIG:Debug>:AETHER_DEBUG>
        $<$<AND:$<CONFIG:Debug>,$<BOOL:${AETHER_VERBOSE_DEBUG}>>:AETHER_VERBOSE_DEBUG>
)

include(cmake/dependencies.cmake)
aether_fetch_dependencies(${AETHER})

if (WIN32)
    if (MINGW OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(${AETHER} PRIVATE
            $<$<CONFIG:Debug>:-mconsole>
            $<$<NOT:$<CONFIG:Debug>>:-mwindows>
        )
    else() # MSVC
        target_link_options(${AETHER} PRIVATE
            $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
            $<$<NOT:$<CONFIG:Debug>>:/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup>
        )
    endif()
endif()

add_custom_command(TARGET ${AETHER} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE-CC-BY-NC-4.0.txt
        $<TARGET_FILE_DIR:${AETHER}>/LICENSE-CC-BY-NC-4.0.txt

    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE-MIT.txt
        $<TARGET_FILE_DIR:${AETHER}>/LICENSE-MIT.txt

    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md
        $<TARGET_FILE_DIR:${AETHER}>/LICENSE.md

    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/THIRD-PARTY-NOTICES.md
        $<TARGET_FILE_DIR:${AETHER}>/THIRD-PARTY-NOTICES.md

    COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE_DIR:${AETHER}>/resources
    
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${AETHER}>/resources
)