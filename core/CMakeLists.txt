file(
  GLOB_RECURSE
  SRCS
  CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(Core ${SRCS})
add_library(${CMAKE_PROJECT_NAME}::Core ALIAS Core)

# remove `lib` prefix
set_target_properties(Core PROPERTIES LINKER_LANGUAGE CXX PREFIX "")

target_link_system_libraries(
  Core
  PRIVATE
  fmt::fmt
  GLEW::GLEW
  glm::glm
  nlohmann_json::nlohmann_json
  SDL3::SDL3
  spdlog::spdlog)

target_link_libraries(Core PRIVATE project_options project_warnings)

target_include_directories(Core ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/core/include>
                                                        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/core/include>)

target_compile_features(Core PUBLIC cxx_std_20)

set_target_properties(
  Core
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(Core PUBLIC CORE_STATIC_DEFINE)
endif()
