include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(project_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project

  if(NOT TARGET fmtlib::fmtlib)
    cpmaddpackage("gh:fmtlib/fmt#10.1.1")
  endif()

  if(NOT TARGET spdlog::spdlog)
    cpmaddpackage(
      GITHUB_REPOSITORY
      gabime/spdlog
      VERSION
      1.12.0
      OPTIONS
      "SPDLOG_FMT_EXTERNAL ON")
  endif()

  if(NOT TARGET Catch2::Catch2WithMain)
    cpmaddpackage("gh:catchorg/Catch2#v3.5.0")
  endif()

  if(NOT TARGET CLI11::CLI11)
    cpmaddpackage("gh:CLIUtils/CLI11#v2.3.2")
  endif()

  if(NOT TARGET SDL2::SDL2)
    cpmaddpackage("gh:libsdl-org/SDL#release-2.28.5")
  endif()

  if(NOT TARGET imgui)
    cpmaddpackage("gh:ocornut/imgui#v1.90")

    add_library(
      imgui STATIC
      ${imgui_SOURCE_DIR}/imgui.cpp
      ${imgui_SOURCE_DIR}/imgui_draw.cpp
      ${imgui_SOURCE_DIR}/imgui_widgets.cpp
      ${imgui_SOURCE_DIR}/imgui_tables.cpp
      ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
      ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)

    target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
    target_compile_definitions(imgui PUBLIC -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS)

    target_link_system_libraries(imgui PRIVATE SDL2::SDL2)
  endif()

  if(NOT TARGET glm::glm)
    cpmaddpackage("gh:g-truc/glm#0.9.9.8")
  endif()

  if(NOT TARGET nlohmann_json::nlohmann_json)
    cpmaddpackage("gh:nlohmann/json#v3.11.3")
  endif()

endfunction()
