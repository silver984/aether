cmake_minimum_required(VERSION 3.31.6)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ae VERSION 1.0 LANGUAGES C CXX)
add_executable(ae)

set(AE_BIN_DIR ${CMAKE_BINARY_DIR}/bin)
set(AE_OUTPUT_DIR ${AE_BIN_DIR}/engine)

set_target_properties(
  ae PROPERTIES
  OUTPUT_NAME "Aether Engine"
  RUNTIME_OUTPUT_DIRECTORY ${AE_OUTPUT_DIR}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/core)
target_link_libraries(ae PRIVATE ae_core)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set_target_properties(ae PROPERTIES WIN32_EXECUTABLE TRUE)
  if(MSVC)
    target_link_options(ae PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(CMAKE_LINKER MATCHES "lld-link|link.exe")
      target_link_options(ae PRIVATE -Wl,/subsystem:windows -Wl,/entry:mainCRTStartup)
    else()
      target_link_options(ae PRIVATE -mwindows)
    endif()
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_options(ae PRIVATE -mwindows)
  endif()
  target_compile_definitions(ae PRIVATE _WINDOWS)
endif()

find_package(Python COMPONENTS Interpreter REQUIRED)

add_custom_command(
  TARGET ae POST_BUILD
  COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/resource_packer.py ${CMAKE_CURRENT_SOURCE_DIR}/resources ${AE_OUTPUT_DIR}/aether
  VERBATIM
)

if(IS_DIRECTORY ${AE_OUTPUT_DIR}/licenses)
  add_custom_command(
    TARGET ae POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${AE_OUTPUT_DIR}/licenses
    VERBATIM
  )
endif()

add_custom_command(
  TARGET ae POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/licenses ${AE_OUTPUT_DIR}/licenses
  VERBATIM
)

add_custom_command(
  TARGET ae POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md ${AE_OUTPUT_DIR}/LICENSE.md
  VERBATIM
)

add_custom_command(
  TARGET ae POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/THIRD-PARTY-NOTICES.md ${AE_OUTPUT_DIR}/THIRD-PARTY-NOTICES.md
  VERBATIM
)