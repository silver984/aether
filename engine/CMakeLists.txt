set(AETHER_ENGINE engine)
set(AETHER_ENGINE_OUTPUT_DIR ${AETHER_OUTPUT_DIR}/${AETHER_ENGINE})

# todo: versioning

add_executable(${AETHER_ENGINE})

set_target_properties(
  ${AETHER_ENGINE}
  PROPERTIES
    OUTPUT_NAME "Aether Engine"
    RUNTIME_OUTPUT_DIRECTORY ${AETHER_ENGINE_OUTPUT_DIR}
)

add_subdirectory(src)
add_subdirectory(external)

target_include_directories(${AETHER_ENGINE} PRIVATE include external)

target_compile_definitions(
  ${AETHER_ENGINE}
  PRIVATE
    $<$<CONFIG:Debug>:AETHER_ENGINE_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:AETHER_ENGINE_RELWITHDEB>
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
list(REMOVE_ITEM LUA_SRC ${lua-src_SOURCE_DIR}/lua.c ${lua-src_SOURCE_DIR}/onelua.c)

add_library(lua STATIC ${LUA_SRC})
target_include_directories(lua PUBLIC ${lua-src_SOURCE_DIR})

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

target_link_libraries(${AETHER_ENGINE} PRIVATE raylib lua sol2::sol2 fmt::fmt)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo")
  set_target_properties(${AETHER_ENGINE} PROPERTIES WIN32_EXECUTABLE TRUE)

  if(MSVC)
    target_link_options(${AETHER_ENGINE} PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)

  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(CMAKE_LINKER MATCHES "lld-link|link.exe")
      target_link_options(${AETHER_ENGINE} PRIVATE -Wl,/subsystem:windows -Wl,/entry:mainCRTStartup)
    else()
      target_link_options(${AETHER_ENGINE} PRIVATE -mwindows)
    endif()

  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_options(${AETHER_ENGINE} PRIVATE -mwindows)
  endif()
    
  target_compile_definitions(${AETHER_ENGINE} PRIVATE _WINDOWS)
endif()

# post build process

find_package(Python COMPONENTS Interpreter REQUIRED)
message(STATUS "Python: ${Python_EXECUTABLE}")

add_custom_command(
  TARGET ${AETHER_ENGINE}
  POST_BUILD
  COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/resource_packer.py
    ${CMAKE_SOURCE_DIR}/resources
    ${AETHER_ENGINE_OUTPUT_DIR}/aether
  VERBATIM
)

if(IS_DIRECTORY ${AETHER_ENGINE_OUTPUT_DIR}/licenses)
  add_custom_command(
    TARGET ${AETHER_ENGINE}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf
      ${AETHER_ENGINE_OUTPUT_DIR}/licenses
    VERBATIM
  )
endif()

add_custom_command(
  TARGET ${AETHER_ENGINE}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/licenses
    ${AETHER_ENGINE_OUTPUT_DIR}/licenses
  VERBATIM
)

add_custom_command(
  TARGET ${AETHER_ENGINE}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/LICENSE.md
    ${AETHER_ENGINE_OUTPUT_DIR}/LICENSE.md
  VERBATIM
)

add_custom_command(
  TARGET ${AETHER_ENGINE}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/THIRD-PARTY-NOTICES.md
    ${AETHER_ENGINE_OUTPUT_DIR}/THIRD-PARTY-NOTICES.md
  VERBATIM
)