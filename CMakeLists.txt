# AetherEngine

cmake_minimum_required(VERSION 4.3.1)

set(aether AetherEngine)
project(${aether} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${aether})

# source files
add_subdirectory(aether)

# includes
target_include_directories(${aether}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include(cmake/dependencies.cmake)
aether_fetch_dependencies(${aether})

if (WIN32)
    # only allow the console on debug
    target_link_options(${aether}
        PRIVATE
            $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
            $<$<CONFIG:Release,RelWithDebInfo,MinSizeRel>:/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup>
    )
endif()

# add_custom_command(TARGET ${aether} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE_DIR:${aether}>/resources
#     COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${aether}>/resources
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${aether}> $<TARGET_FILE_DIR:${aether}>
#     COMMAND_EXPAND_LISTS
# )