cmake_minimum_required(VERSION 3.15...3.23)

project(CmakeConfigPackageTests LANGUAGES CXX)

# ---- Test as standalone project the exported config package ----

if(PROJECT_IS_TOP_LEVEL OR TEST_INSTALLED_VERSION)
  enable_testing()

  find_package(${CMAKE_PROJECT_NAME} CONFIG REQUIRED)

  if(NOT TARGET project_options)
    message(FATAL_ERROR "Requiered config package not found!")
    return() # be strictly paranoid for Template Janitor github action! CK
  endif()
endif()

# ---- Dependencies ----

include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)

# Provide a simple smoke test to make sure that the CLI works and can display a
# --help message
add_test(NAME cli.has_help COMMAND morse --help)

# Provide a test to verify that the version being reported from the application
# matches the version given to CMake. This will be important once you package
# your program. Real world shows that this is the kind of simple mistake that is
# easy to make, but also easy to test for.
add_test(NAME cli.version_matches COMMAND morse --version)
set_tests_properties(cli.version_matches PROPERTIES PASS_REGULAR_EXPRESSION "${PROJECT_VERSION}")

# ---- Test binaries ----

add_executable(tests tests.cpp)
target_link_libraries(
  tests
  PRIVATE ${CMAKE_PROJECT_NAME}::project_warnings
          ${CMAKE_PROJECT_NAME}::project_options
          ${CMAKE_PROJECT_NAME}::lib${CMAKE_PROJECT_NAME})

target_link_system_libraries(
  tests
  PRIVATE Catch2::Catch2WithMain
          GLEW::GLEW
          glm::glm
          nlohmann_json::nlohmann_json
          SDL2
          spdlog::spdlog
          OpenGL::OpenGL)

if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(
    TARGET tests
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:tests> $<TARGET_FILE_DIR:tests>
    COMMAND_EXPAND_LISTS)
endif()

# automatically discover tests that are defined in catch based test files you
# can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
  tests
  TEST_PREFIX
  "unittests."
  REPORTER
  XML
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "unittests."
  OUTPUT_SUFFIX
  .xml)

# Add a file containing a set of constexpr tests
add_executable(constexpr_tests constexpr_tests.cpp)
target_link_libraries(
  constexpr_tests
  PRIVATE ${CMAKE_PROJECT_NAME}::project_warnings
          ${CMAKE_PROJECT_NAME}::project_options
          ${CMAKE_PROJECT_NAME}::lib${CMAKE_PROJECT_NAME}
          Catch2::Catch2WithMain)

catch_discover_tests(
  constexpr_tests
  TEST_PREFIX
  "constexpr."
  REPORTER
  XML
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "constexpr."
  OUTPUT_SUFFIX
  .xml)

# Disable the constexpr portion of the test, and build again this allows us to
# have an executable that we can debug when things go wrong with the constexpr
# testing
add_executable(relaxed_constexpr_tests constexpr_tests.cpp)
target_link_libraries(
  relaxed_constexpr_tests
  PRIVATE ${CMAKE_PROJECT_NAME}::project_warnings
          ${CMAKE_PROJECT_NAME}::project_options
          ${CMAKE_PROJECT_NAME}::lib${CMAKE_PROJECT_NAME}
          Catch2::Catch2WithMain)
target_compile_definitions(relaxed_constexpr_tests PRIVATE -DCATCH_CONFIG_RUNTIME_STATIC_REQUIRE)

catch_discover_tests(
  relaxed_constexpr_tests
  TEST_PREFIX
  "relaxed_constexpr."
  REPORTER
  XML
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "relaxed_constexpr."
  OUTPUT_SUFFIX
  .xml)

if(NOT TARGET ${CMAKE_PROJECT_NAME}_tests)
  add_custom_target(${CMAKE_PROJECT_NAME}_tests)
  add_dependencies(
    ${CMAKE_PROJECT_NAME}_tests
    tests
    constexpr_tests
    relaxed_constexpr_tests)
endif()
