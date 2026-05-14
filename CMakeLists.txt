# AetherEngine

cmake_minimum_required(VERSION 3.31.6)

set(AETHER AetherEngine)
include(cmake/version.cmake)
project(${AETHER} VERSION ${AETHER_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(AETHER_VERBOSE_LOGS "Enable verbose debug logs" OFF)

add_executable(${AETHER})

# source files
add_subdirectory(aether)
add_subdirectory(external)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/aether/version.h
    @ONLY
)

# includes
target_include_directories(${AETHER}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_compile_definitions(${AETHER}
    PRIVATE
        $<$<CONFIG:Debug>:AETHER_DEBUG>
        $<$<BOOL:${AETHER_VERBOSE_LOGS}>:AETHER_VERBOSE_LOGS>
)

include(cmake/dependencies.cmake)
aether_fetch_dependencies(${AETHER})

if (WIN32)
    # MinGW (GCC)
    if (MINGW OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(${AETHER} PRIVATE
            $<$<CONFIG:Debug>:-mconsole>
            $<$<NOT:$<CONFIG:Debug>>:-mwindows>
        )
    else()
        # MSVC
        target_link_options(${AETHER} PRIVATE
            $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
            $<$<NOT:$<CONFIG:Debug>>:/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup>
        )
    endif()
endif()

add_custom_command(TARGET ${AETHER} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE_DIR:${AETHER}>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${AETHER}>/resources
)