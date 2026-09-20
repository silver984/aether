add_executable(aether.engine)

set(AETHER_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set(AETHER_OUTPUT_DIR ${AETHER_BINARY_DIR}/engine)

set_target_properties(
  aether.engine PROPERTIES
  OUTPUT_NAME "Aether Engine"
  RUNTIME_OUTPUT_DIRECTORY ${AETHER_OUTPUT_DIR}
)

target_include_directories(aether.engine PRIVATE include)

file(GLOB AETHER_SOURCE CONFIGURE_DEPENDS src/*.c*)
file(GLOB AETHER_NODE_COMPONENTS_SOURCE CONFIGURE_DEPENDS src/node/components/*.c*)

target_sources(
  aether.engine PRIVATE
  src/node/node.cc
  ${AETHER_SOURCE}
  ${AETHER_NODE_COMPONENTS_SOURCE}
)

add_subdirectory(vendor)
include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage(
  NAME raylib
  GITHUB_REPOSITORY raysan5/raylib
  GIT_TAG 6.0
  GIT_SHALLOW TRUE
)

target_compile_definitions(
  raylib PRIVATE
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
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 12.1.0
  GIT_SHALLOW TRUE
)

target_link_libraries(aether.engine PUBLIC raylib)
add_subdirectory(modules)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set_target_properties(aether.engine PROPERTIES WIN32_EXECUTABLE TRUE)
  if(MSVC)
    target_link_options(aether.engine PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(CMAKE_LINKER MATCHES "lld-link|link.exe")
      target_link_options(aether.engine PRIVATE -Wl,/subsystem:windows -Wl,/entry:mainCRTStartup)
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
  TARGET aether.engine POST_BUILD
  COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/resource_packer.py ${CMAKE_SOURCE_DIR}/resources ${AETHER_OUTPUT_DIR}/aether
  VERBATIM
)

if(IS_DIRECTORY ${AETHER_OUTPUT_DIR}/licenses)
  add_custom_command(
    TARGET aether.engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${AETHER_OUTPUT_DIR}/licenses
    VERBATIM
  )
endif()

add_custom_command(
  TARGET aether.engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/licenses ${AETHER_OUTPUT_DIR}/licenses
  VERBATIM
)

add_custom_command(
  TARGET aether.engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/LICENSE.md ${AETHER_OUTPUT_DIR}/LICENSE.md
  VERBATIM
)

add_custom_command(
  TARGET aether.engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/THIRD-PARTY-NOTICES.md ${AETHER_OUTPUT_DIR}/THIRD-PARTY-NOTICES.md
  VERBATIM
)

