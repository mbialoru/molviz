#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <cmath>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <spdlog/spdlog.h>

#include "gfx/parsergltf.hpp"

using mve::gfx::Vertex;
using mve::gfx::Shader;
using mve::gfx::Mesh;
using mve::gfx::Model;
using mve::gfx::Camera;
using mve::gfx::ParserGLTF;

// main code
int main(int argc, char **argv)
{
  // set loglevel
  spdlog::set_level(spdlog::level::debug);

  // setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
    spdlog::error("SDL initialization error: {}", SDL_GetError());
    return -1;
  }

  // decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char *glsl_version = "#version 100";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
  // GL 3.2 Core + GLSL 150
  const char *glsl_version = "#version 150";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);// Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
  // from 2.0.18: enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

  // create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_WindowFlags window_flags =
    static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  SDL_Window *window =
    SDL_CreateWindow("Molviz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, window_flags);
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);

  // try to enable vsync
  if (SDL_GL_SetSwapInterval(1) < 0) { spdlog::warn("unable to set VSync! SDL error: {}", SDL_GetError()); }

  // initialize GLEW
  GLenum status{ glewInit() };
  if (status != GLEW_OK) {
    std::string error_message{ reinterpret_cast<const char *>(glewGetErrorString(status)) };
    spdlog::error("GLEW init failed! error:{}", error_message);
    return -1;
  }

  // create Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();

  // enable keyboard controls for ImGui
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  // set color style
  ImGui::StyleColorsDark();

  // setup platform/renderer backends
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // window size
  int window_width;
  int window_height;
  SDL_GetWindowSize(window, &window_width, &window_height);
  spdlog::debug("SDL window size {}x{}", window_width, window_height);

  // our application state
  // TODO: separate structs for storing context information
  ImVec4 clear_color = ImVec4(0.5F, 0.0F, 1.0F, 1.0F);

  spdlog::debug("imgui configured");

  std::filesystem::path fragment_shader{ "../resources/shaders/default.frag" };
  std::filesystem::path vertex_shader{ "../resources/shaders/default.vert" };

  Shader shader_program{ vertex_shader, fragment_shader };
  shader_program.activate();

  glm::vec4 light_color{ 0.5F, 0.5F, 0.5F, 0.5F };
  glm::vec3 light_position{ 0.0F, 0.0F, 1.0F };
  glUniform4f(glGetUniformLocation(shader_program.id, "u_light_color"),
    light_color.r,
    light_color.g,
    light_color.b,
    light_color.a);
  glUniform3f(
    glGetUniformLocation(shader_program.id, "u_light_position"), light_position.x, light_position.y, light_color.z);

  // clipping of overlapping vertices, depth buffer or "z buffer"
  glEnable(GL_DEPTH_TEST);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);// wireframe

  // create camera
  Camera camera(static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y), glm::vec3(0.0F, 0.0F, 0.0F));

  // create models
  std::filesystem::path model_gltf_file{ "../resources/meshes/cube_color/cube_color.gltf" };
  ParserGLTF parser{ std::filesystem::absolute(model_gltf_file) };
  Model model{ parser.get_model() };

  // main loop
  bool done = false;

  uint32_t this_frametime{ 0 };

  while (!done) {
    uint32_t last_frametime{ this_frametime };
    this_frametime = SDL_GetTicks();

    // Poll and handle events (inputs, window resize, etc.)
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT) { done = true; }
      if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE
          && event.window.windowID == SDL_GetWindowID(window)) {
        done = true;
      }
      camera.handle_inputs(event, window);
    }

    // start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // handle viewport content rendering
    ImGui::Render();
    glViewport(0, 0, static_cast<GLsizei>(io.DisplaySize.x), static_cast<GLsizei>(io.DisplaySize.y));
    glClearColor(
      clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.update_matrix(45.0F, 0.1F, 100.0F);
    model.draw(shader_program, camera);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);

    // fps limiter
    if (this_frametime - last_frametime < 1000 / 60) SDL_Delay(1000 / 60 - this_frametime + last_frametime);
  }

  // cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  shader_program.cleanup();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
