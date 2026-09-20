add_executable(aether_engine_log_sample ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc)
set_target_properties(
  aether_engine_log_sample PROPERTIES
  OUTPUT_NAME "log"
  RUNTIME_OUTPUT_DIRECTORY ${AE_SAMPLES_DIR}
)
target_link_libraries(aether_engine_log_sample PRIVATE aether_engine_log)