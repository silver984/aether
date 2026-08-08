cmake_minimum_required(VERSION 3.31.6)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(AETHER aether)
set(AETHER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)

project(${AETHER} VERSION 1.0 LANGUAGES C CXX)

add_subdirectory(engine)