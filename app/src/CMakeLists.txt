if(NOT TARGET ${CMAKE_PROJECT_NAME})

  add_executable(${CMAKE_PROJECT_NAME} main.cpp)

  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}::project_options
                                                      ${CMAKE_PROJECT_NAME}::project_warnings Core)

  target_link_system_libraries(
    ${CMAKE_PROJECT_NAME}
    PRIVATE
    CLI11::CLI11
    fmt::fmt
    GLEW::GLEW
    glm::glm
    imgui
    nlohmann_json::nlohmann_json
    OpenGL::OpenGL
    SDL2
    spdlog::spdlog)

  target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${CMAKE_BINARY_DIR}/app/include")

  add_dependencies(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_resources)

endif()
