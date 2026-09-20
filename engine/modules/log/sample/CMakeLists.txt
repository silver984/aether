add_executable(aether.engine-log-sample src/main.cc)
set_target_properties(
  aether.engine-log-sample PROPERTIES
  OUTPUT_NAME "log"
  RUNTIME_OUTPUT_DIRECTORY ${AETHER_SAMPLES_DIR}
)
target_link_libraries(aether.engine-log-sample PRIVATE aether.engine-log)