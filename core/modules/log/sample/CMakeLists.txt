add_executable(aether.logsample ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc)
set_target_properties(
  aether.logsample PROPERTIES
  OUTPUT_NAME "log"
  RUNTIME_OUTPUT_DIRECTORY ${AE_SAMPLES_DIR}
)
target_link_libraries(aether.logsample PRIVATE ae_log)