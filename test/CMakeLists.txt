# ---- Dependencies ----

include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)

# ---- Tests ----

if(NOT TARGET tests)
  file(
    GLOB
    TESTS_SRCS
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_*.cpp)

  add_executable(tests ${TESTS_SRCS})
  target_include_directories(tests PRIVATE include)
  target_link_libraries(
    tests
    PRIVATE ${CMAKE_PROJECT_NAME}::project_warnings
            ${CMAKE_PROJECT_NAME}::project_options
            ${CMAKE_PROJECT_NAME}::Core)

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

# ---- Constexpr tests ----

# add a file containing a set of constexpr tests
if(NOT TARGET constexpr_tests)
  add_executable(constexpr_tests src/constexpr_tests.cpp)
  target_include_directories(constexpr_tests PRIVATE include)
  target_link_libraries(
    constexpr_tests
    PRIVATE ${CMAKE_PROJECT_NAME}::project_warnings
            ${CMAKE_PROJECT_NAME}::project_options
            ${CMAKE_PROJECT_NAME}::Core
            Catch2::Catch2WithMain)
endif()

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

# disable the constexpr portion of the test, and build again this allows us to
# have an executable that we can debug when things go wrong with the constexpr
# testing
if(NOT TARGET relaxed_constexpr_tests)
  add_executable(relaxed_constexpr_tests src/constexpr_tests.cpp)
  target_include_directories(relaxed_constexpr_tests PRIVATE include)
  target_link_libraries(
    relaxed_constexpr_tests
    PRIVATE ${CMAKE_PROJECT_NAME}::project_warnings
            ${CMAKE_PROJECT_NAME}::project_options
            ${CMAKE_PROJECT_NAME}::Core
            Catch2::Catch2WithMain)
  target_compile_definitions(relaxed_constexpr_tests PRIVATE -DCATCH_CONFIG_RUNTIME_STATIC_REQUIRE)
endif()

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

if(NOT TARGET all_tests)
  add_custom_target(all_tests)
  add_dependencies(
    tests
    constexpr_tests
    relaxed_constexpr_tests)
endif()
