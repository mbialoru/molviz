add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}::project_options
                                                    ${CMAKE_PROJECT_NAME}::project_warnings)

target_link_system_libraries(
  ${CMAKE_PROJECT_NAME}
  PRIVATE
  CLI11::CLI11
  fmt::fmt
  spdlog::spdlog)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${CMAKE_BINARY_DIR}/config/include")
