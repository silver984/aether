# AetherEngine

cmake_minimum_required(VERSION 4.3.1)

include(cmake/version.cmake)
project(Aether VERSION ${AETHER_VERSION} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(AETHER_VERBOSE_DEBUG "Enable verbose debug" OFF)

add_executable(${PROJECT_NAME})

# source files
add_subdirectory(aether)
add_subdirectory(external)

configure_file(
    cmake/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/version.h
    @ONLY
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    include
    external
    ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    $<$<CONFIG:Debug>:AETHER_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:AETHER_RELWITHDEB>
    $<$<BOOL:${AETHER_VERBOSE_DEBUG}>:AETHER_VERBOSE_DEBUG>
)

target_precompile_headers(
    ${PROJECT_NAME}
    PRIVATE
    <sol/sol.hpp>
)

include(cmake/dependencies.cmake)
aether_fetch_dependencies(${PROJECT_NAME})

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        target_link_options(
            ${PROJECT_NAME}
            PRIVATE
            $<$<CONFIG:Debug>:-mconsole>
            $<$<NOT:$<CONFIG:Debug>>:-mwindows>
        )
    else()
        target_link_options(
            ${PROJECT_NAME}
            PRIVATE
            $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
            $<$<NOT:$<CONFIG:Debug>>:/SUBSYSTEM:WINDOWS>
            $<$<NOT:$<CONFIG:Debug>>:/ENTRY:mainCRTStartup>
        )
    endif()
endif()

set(AETHER_OUTPUT_DIR $<TARGET_FILE_DIR:${PROJECT_NAME}>)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E tar cf ${AETHER_OUTPUT_DIR}/aether.res --format=zip .
    WORKING_DIRECTORY resources
    COMMENT "Compressing resources"
    VERBATIM
)

# using -E remove_directory doesnt throw an error if the directory doesnt exist yet
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${AETHER_OUTPUT_DIR}/licenses
    COMMENT "Removing old licenses folder"
    VERBATIM
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        licenses
        ${AETHER_OUTPUT_DIR}/licenses
    COMMENT "Copying licenses folder"
    VERBATIM
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        LICENSE.md
        ${AETHER_OUTPUT_DIR}/LICENSE.md
    COMMENT "Copying license markdown"
    VERBATIM
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        THIRD-PARTY-NOTICES.md
        ${AETHER_OUTPUT_DIR}/THIRD-PARTY-NOTICES.md
    COMMENT "Copying third-party-notices markdown"
    VERBATIM
)