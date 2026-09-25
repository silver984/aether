add_executable(log-sample src/main.cc)
set_target_properties(
  log-sample PROPERTIES
  OUTPUT_NAME "log"
  RUNTIME_OUTPUT_DIRECTORY ${AETHER_SAMPLES_DIR}
)
target_link_libraries(log-sample PRIVATE aether-engine::log)