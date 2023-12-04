add_subdirectory(src)
add_subdirectory(config)

add_custom_target(
  ${CMAKE_PROJECT_NAME}_resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory /workspaces/${CMAKE_PROJECT_NAME}/app/${CMAKE_PROJECT_NAME}/resources ${CMAKE_BINARY_DIR}/resources
  COMMENT "Copying resources"
  VERBATIM)