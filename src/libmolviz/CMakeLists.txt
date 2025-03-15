file(
  GLOB
  SRCS
  CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(lib${CMAKE_PROJECT_NAME} ${SRCS})
add_library(${CMAKE_PROJECT_NAME}::lib${CMAKE_PROJECT_NAME} ALIAS lib${CMAKE_PROJECT_NAME})

# remove `lib` prefix
set_target_properties(lib${CMAKE_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX PREFIX "")

add_subdirectory(gfx)

target_link_system_libraries(
  lib${CMAKE_PROJECT_NAME}
  PRIVATE
  fmt::fmt
  GLEW::GLEW
  glm::glm
  nlohmann_json::nlohmann_json
  SDL2
  spdlog::spdlog)

target_link_libraries(lib${CMAKE_PROJECT_NAME} PRIVATE project_options project_warnings)

target_include_directories(
  lib${CMAKE_PROJECT_NAME} ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                   $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(lib${CMAKE_PROJECT_NAME} PUBLIC cxx_std_20)

set_target_properties(
  lib${CMAKE_PROJECT_NAME}
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(lib${CMAKE_PROJECT_NAME} PUBLIC lib${CMAKE_PROJECT_NAME}_STATIC_DEFINE)
endif()
