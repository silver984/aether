add_library(aether_engine_core STATIC)

set(AE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(AE_VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(AE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE AE_HEADERS CONFIGURE_DEPENDS ${AE_INCLUDE_DIR}/aether/*.h*)
list(FILTER AE_HEADERS EXCLUDE REGEX ^${AE_INCLUDE_DIR}/aether/excluded/)

file(GLOB_RECURSE AE_VENDOR_HEADERS CONFIGURE_DEPENDS ${AE_VENDOR_DIR}/*.h*)
# exclude this folder from `AE_VENDOR_HEADERS` since ive written soloud's lib inside vendor which should take care of it
list(FILTER AE_VENDOR_HEADERS EXCLUDE REGEX ^${AE_VENDOR_DIR}/soloud/)

file(GLOB AE_SRC CONFIGURE_DEPENDS ${AE_SRC_DIR}/*.c*)
file(GLOB AE_NODE_COMP_SRC CONFIGURE_DEPENDS ${AE_SRC_DIR}/node/components/*.c*)

target_sources(
  aether_engine_core
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      ${AE_INCLUDE_DIR}
      ${AE_VENDOR_DIR}
    FILES
      ${AE_HEADERS}
      ${AE_VENDOR_HEADERS}
  PRIVATE
    ${AE_SRC_DIR}/node/node.cc
    ${AE_SRC}
    ${AE_NODE_COMP_SRC}
)

add_subdirectory(${AE_VENDOR_DIR})
include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

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

# maybe todo: just vendor lua?
CPMAddPackage(
  NAME lua
  GITHUB_REPOSITORY lua/lua
  GIT_TAG v5.4.8
  GIT_SHALLOW TRUE
)

add_library(lua STATIC)

file(GLOB LUA_HEADERS ${lua_SOURCE_DIR}/*.h)
file(GLOB LUA_SRC ${lua_SOURCE_DIR}/*.c)

list(
  REMOVE_ITEM LUA_SRC
    ${lua_SOURCE_DIR}/lua.c
    ${lua_SOURCE_DIR}/onelua.c
)

target_sources(
  lua
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${lua_SOURCE_DIR}
    FILES ${LUA_HEADERS}
  PRIVATE ${LUA_SRC}
)

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

target_link_libraries(aether_engine_core PUBLIC soloud raylib lua sol2::sol2)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules)