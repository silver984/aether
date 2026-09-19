cmake_minimum_required(VERSION 3.31.6)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(aether.engine VERSION 1.0 LANGUAGES C CXX)
add_executable(aether.engine)

set(AE_BIN_DIR ${CMAKE_BINARY_DIR}/bin)
set(AE_SAMPLES_DIR ${AE_BIN_DIR}/samples)

option(AE_BUILD_ALL_MODULE_SAMPLES "Build all module samples" OFF)

set_target_properties(
  aether.engine PROPERTIES
  OUTPUT_NAME "Aether Engine"
  RUNTIME_OUTPUT_DIRECTORY ${AE_BIN_DIR}/engine
)

file(GLOB AE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*)
file(GLOB AE_NODE_COMP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/node/components/*.c*)

target_sources(
  aether.engine PRIVATE
  src/node/node.cc
  ${AE_SRC} ${AE_NODE_COMP_SRC}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules)

target_include_directories(
  aether.engine PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage(
  NAME raylib
  GITHUB_REPOSITORY raysan5/raylib
  GIT_TAG 6.0
  GIT_SHALLOW TRUE
)

target_compile_definitions(
  raylib PRIVATE
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

CPMAddPackage(
  NAME lua_src
  GITHUB_REPOSITORY lua/lua
  GIT_TAG v5.4.8
  GIT_SHALLOW TRUE
)

file(GLOB LUA_SRC ${lua_src_SOURCE_DIR}/*.c)

list(
  REMOVE_ITEM LUA_SRC
  ${lua_src_SOURCE_DIR}/lua.c
  ${lua_src_SOURCE_DIR}/onelua.c
)

add_library(lua STATIC ${LUA_SRC})
target_include_directories(lua PUBLIC ${lua_src_SOURCE_DIR})

set(SOL2_BUILD_LUA OFF)
set(SOL2_LUA_VERSION "5.4.8")

CPMAddPackage(
  NAME sol2
  GITHUB_REPOSITORY ThePhD/sol2
  GIT_TAG v3.5.0
  GIT_SHALLOW TRUE
)

CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 12.1.0
  GIT_SHALLOW TRUE
)

target_link_libraries(
  aether.engine PRIVATE
  raylib lua
  sol2::sol2 fmt::fmt
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set_target_properties(
    aether.engine PROPERTIES
    WIN32_EXECUTABLE TRUE
  )
  if(MSVC)
    target_link_options(
      aether.engine PRIVATE
      /SUBSYSTEM:WINDOWS
      /ENTRY:mainCRTStartup
    )
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(CMAKE_LINKER MATCHES "lld-link|link.exe")
      target_link_options(
        aether.engine PRIVATE
        -Wl,/subsystem:windows
        -Wl,/entry:mainCRTStartup
      )
    else()
      target_link_options(aether.engine PRIVATE -mwindows)
    endif()
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_options(aether.engine PRIVATE -mwindows)
  endif()
  target_compile_definitions(aether.engine PRIVATE _WINDOWS)
endif()

find_package(Python COMPONENTS Interpreter REQUIRED)

add_custom_command(
  TARGET aether.engine
  POST_BUILD
  COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/resource_packer.py
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${AE_OUTPUT_DIR}/aether
  VERBATIM
)

if(IS_DIRECTORY ${AE_OUTPUT_DIR}/licenses)
  add_custom_command(
    TARGET aether.engine
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf
      ${AE_OUTPUT_DIR}/licenses
    VERBATIM
  )
endif()

add_custom_command(
  TARGET aether.engine
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/licenses
    ${AE_OUTPUT_DIR}/licenses
  VERBATIM
)

add_custom_command(
  TARGET aether.engine
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md
    ${AE_OUTPUT_DIR}/LICENSE.md
  VERBATIM
)

add_custom_command(
  TARGET aether.engine
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/THIRD-PARTY-NOTICES.md
    ${AE_OUTPUT_DIR}/THIRD-PARTY-NOTICES.md
  VERBATIM
)