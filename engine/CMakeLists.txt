set(AETHER_ENGINE engine)

# todo: versioning

add_executable(${AETHER_ENGINE})

set_target_properties(
  ${AETHER_ENGINE}
  PROPERTIES
    OUTPUT_NAME "Aether Engine"
    RUNTIME_OUTPUT_DIRECTORY ${AETHER_OUTPUT_DIR}/engine
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external)

target_include_directories(
  ${AETHER_ENGINE}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)
    
option(AETHER_VERBOSE_DEBUG "Enable verbose debug" OFF)

target_compile_definitions(
  ${AETHER_ENGINE}
  PRIVATE
    $<$<CONFIG:Debug>:AETHER_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:AETHER_RELWITHDEB>
    $<$<AND:$<CONFIG:Debug>,$<BOOL:${AETHER_VERBOSE_DEBUG}>>:AETHER_VERBOSE_DEBUG>
)

include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

# raylib
CPMAddPackage(
  NAME raylib
  GITHUB_REPOSITORY raysan5/raylib
  GIT_TAG 6.0
  GIT_SHALLOW TRUE
)

target_compile_definitions(
  raylib
  PRIVATE
    SUPPORT_FILEFORMAT_JPG=1
    SUPPORT_MODULE_RAUDIO=0
    SUPPORT_TRACELOG=0
    SUPPORT_SCREEN_CAPTURE=0
    SUPPORT_CAMERA_SYSTEM=0
    SUPPORT_GESTURES_SYSTEM=0
    SUPPORT_RPRAND_GENERATOR=0
    SUPPORT_WINMM_HIGHRES_TIMER=0
    SUPPORT_CLIPBOARD_IMAGE=0
    SUPPORT_FILEFORMAT_OBJ=0
    SUPPORT_FILEFORMAT_MTL=0
    SUPPORT_FILEFORMAT_IQM=0
    SUPPORT_FILEFORMAT_GLTF=0
    SUPPORT_FILEFORMAT_VOX=0
    SUPPORT_FILEFORMAT_M3D=0
    SUPPORT_MESH_GENERATION=0
)

# lua
CPMAddPackage(
  NAME lua-src
  GITHUB_REPOSITORY lua/lua
  GIT_TAG v5.4.8
  GIT_SHALLOW TRUE
)

file(GLOB LUA_SRC ${lua-src_SOURCE_DIR}/*.c)

list(
  REMOVE_ITEM
  LUA_SRC
    ${lua-src_SOURCE_DIR}/lua.c
    ${lua-src_SOURCE_DIR}/onelua.c
)

add_library(
  lua
  STATIC
    ${LUA_SRC}
)

target_include_directories(
  lua
  PUBLIC
    ${lua-src_SOURCE_DIR}
)

# sol2
set(SOL2_BUILD_LUA OFF)
set(SOL2_LUA_VERSION "5.4.8")

CPMAddPackage(
  NAME sol2
  GITHUB_REPOSITORY ThePhD/sol2
  GIT_TAG v3.5.0
  GIT_SHALLOW TRUE
)

# fmt
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 12.1.0
  GIT_SHALLOW TRUE
)

target_link_libraries(
  ${AETHER_ENGINE}
  PRIVATE
    raylib
    lua
    sol2::sol2
    fmt::fmt
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    target_link_options(
      ${AETHER_ENGINE}
      PRIVATE
        $<$<CONFIG:Debug>:-mconsole>
        $<$<NOT:$<CONFIG:Debug>>:-mwindows>
    )
  else()
    target_link_options(
      ${AETHER_ENGINE}
      PRIVATE
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
        $<$<NOT:$<CONFIG:Debug>>:/SUBSYSTEM:WINDOWS>
        $<$<NOT:$<CONFIG:Debug>>:/ENTRY:mainCRTStartup>
    )
  endif()
endif()

# set(AETHER_OUTPUT_DIR $<TARGET_FILE_DIR:${AETHER_ENGINE}>)

# # using -E remove_directory doesnt throw an error if the directory doesnt exist yet
# add_custom_command(
#   TARGET ${AETHER_ENGINE}
#   POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E remove_directory ${AETHER_OUTPUT_DIR}/licenses
#   COMMENT "Removing old licenses folder"
#   VERBATIM
# )

# add_custom_command(
#   TARGET ${AETHER_ENGINE}
#   POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy_directory
#     licenses
#     ${AETHER_OUTPUT_DIR}/licenses
#   COMMENT "Copying licenses folder"
#   VERBATIM
# )

# add_custom_command(
#   TARGET ${AETHER_ENGINE}
#   POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy
#     LICENSE.md
#     ${AETHER_OUTPUT_DIR}/LICENSE.md
#   COMMENT "Copying license markdown"
#   VERBATIM
# )

# add_custom_command(
#   TARGET ${AETHER_ENGINE}
#   POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy
#     THIRD-PARTY-NOTICES.md
#     ${AETHER_OUTPUT_DIR}/THIRD-PARTY-NOTICES.md
#   COMMENT "Copying third-party-notices markdown"
#   VERBATIM
# )