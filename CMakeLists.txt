# AetherEngine
cmake_minimum_required(VERSION 3.31.6)

include(cmake/version.cmake)
project(Aether VERSION ${AETHER_VERSION} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(AETHER_VERBOSE_DEBUG "Enable verbose debug" OFF)

add_executable(${PROJECT_NAME})

# source files
add_subdirectory(aether)
add_subdirectory(external)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/version.h
    @ONLY
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:AETHER_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:AETHER_RELWITHDEB>
    $<$<BOOL:${AETHER_VERBOSE_DEBUG}>:AETHER_VERBOSE_DEBUG>
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    <sol/sol.hpp>
)

include(cmake/dependencies.cmake)
aether_fetch_dependencies(${PROJECT_NAME})

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (MINGW OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:-mconsole>
            $<$<NOT:$<CONFIG:Debug>>:-mwindows>
        )
    else() # MSVC
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
            $<$<NOT:$<CONFIG:Debug>>:/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup>
        )
    endif()
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md $<TARGET_FILE_DIR:${PROJECT_NAME}>/LICENSE.md
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/THIRD-PARTY-NOTICES.md $<TARGET_FILE_DIR:${PROJECT_NAME}>/THIRD-PARTY-NOTICES.md
    COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/licenses
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/licenses $<TARGET_FILE_DIR:${PROJECT_NAME}>/licenses
)