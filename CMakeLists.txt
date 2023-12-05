cmake_minimum_required(VERSION 3.21)

# this template attempts to be "fetch_content"-able
# so that it works well with tools like CPM or other
# manual dependency management

# only set the cxx_standard if it is not set by someone else
if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

# set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# add git variables
include(${CMAKE_SOURCE_DIR}/cmake/GitVars.cmake)

# set the project name and language
project(
  molviz
  VERSION 0.0.1
  DESCRIPTION ""
  HOMEPAGE_URL "%%myurl%%"
  LANGUAGES CXX C)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include(cmake/PreventInSourceBuilds.cmake)
include(ProjectOptions.cmake)

project_setup_options()
project_global_options()

include(Dependencies.cmake)

project_setup_dependencies()
project_local_options()

# don't know if this should be set globally from here or not...
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

target_compile_features(project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

add_library(${CMAKE_PROJECT_NAME}::project_options ALIAS project_options)
add_library(${CMAKE_PROJECT_NAME}::project_warnings ALIAS project_warnings)

# add our project code
add_subdirectory(core)
add_subdirectory(app)

# don't even look at tests if we're not top level
if(NOT PROJECT_IS_TOP_LEVEL)
  return()
endif()

# add tests
include(CTest)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

# if MSVC is being used, and asan is enabled, we need to set the debugger
# environment so that it behaves well with MSVC's debugger, and we can run the
# target from visual studio
if(MSVC)
  get_all_installable_targets(all_targets)
  message("all_targets=${all_targets}")
  set_target_properties(${all_targets} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=$(VC_ExecutablePath_x64);%PATH%")
endif()

# set the startup project for the "play" button in MSVC / VisualStudio
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})

if(CMAKE_SKIP_INSTALL_RULES)
  return()
endif()

include(cmake/PackageProject.cmake)

# add other targets that you want installed here, by default we just package the
# one executable we know we want to ship
project_package_project(
  TARGETS
  ${CMAKE_PROJECT_NAME}
  project_options
  project_warnings
  # FIXME: this does not work! CK
  # PRIVATE_DEPENDENCIES_CONFIGURED project_options project_warnings
)

# experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you
# track a build to a specific GIT SHA
set(CPACK_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

message(STATUS "PACKAGE NAME: ${CPACK_PACKAGE_FILE_NAME}")

include(CPack)
