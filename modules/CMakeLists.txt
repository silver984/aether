add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/log)
target_link_libraries(aether.engine PRIVATE aether.log)