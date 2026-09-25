add_executable(ref-sample src/main.cc)
set_target_properties(
  ref-sample PROPERTIES
  OUTPUT_NAME "ref"
  RUNTIME_OUTPUT_DIRECTORY ${AETHER_SAMPLES_DIR}
)
target_link_libraries(
  ref-sample PRIVATE
  aether-engine::log
  aether-engine::ref
)