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

#include "gfx/mesh.hpp"

using Molviz::gfx::Vertex;
using Molviz::gfx::Shader;
using Molviz::gfx::Mesh;
using Molviz::gfx::Camera;

// Main code
int main(int argc, char **argv)
{
  // set loglevel
  spdlog::set_level(spdlog::level::debug);

  // setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
    spdlog::error("SDL initialization error: {}", SDL_GetError());
    return -1;
  }
  // Decide GL+GLSL versions
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
  // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  SDL_Window *window =
    SDL_CreateWindow("Molviz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, window_flags);
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  glewInit();
  SDL_GL_MakeCurrent(window, gl_context);

  // Use Vsync
  if (SDL_GL_SetSwapInterval(1) < 0) { spdlog::warn("unable to set VSync! SDL error: {}", SDL_GetError()); }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Our state
  ImVec4 clear_color = ImVec4(1.00F, 1.00F, 1.00F, 1.00F);

  spdlog::debug("imgui configured");

  // Vertices coordinates
  Vertex model_vertices_raw[] = {
    Vertex{ glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.5f, 1.0f) },
    Vertex{ glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.5f, 1.0f) },
    Vertex{ glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.5f, 1.0f) },
    Vertex{ glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.5f, 1.0f) }
  };

  // Indices for model_vertices order
  GLuint model_indices_raw[] = { 0, 1, 2, 0, 2, 3 };

  Vertex light_vertices_raw[] = { Vertex{ glm::vec3(-0.1f, -0.1f, 0.1f) },
    Vertex{ glm::vec3(-0.1f, -0.1f, -0.1f) },
    Vertex{ glm::vec3(0.1f, -0.1f, -0.1f) },
    Vertex{ glm::vec3(0.1f, -0.1f, 0.1f) },
    Vertex{ glm::vec3(-0.1f, 0.1f, 0.1f) },
    Vertex{ glm::vec3(-0.1f, 0.1f, -0.1f) },
    Vertex{ glm::vec3(0.1f, 0.1f, -0.1f) },
    Vertex{ glm::vec3(0.1f, 0.1f, 0.1f) } };

  GLuint light_indices_raw[] = {
    0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 7, 3, 3, 7, 6, 3, 6, 2, 2, 6, 5, 2, 5, 1, 1, 5, 4, 1, 4, 0, 4, 5, 6, 4, 6, 7
  };

  std::filesystem::path model_fragment_shader{ "../resources/shaders/default.frag" };
  std::filesystem::path model_vertex_shader{ "../resources/shaders/default.vert" };

  std::filesystem::path light_fragment_shader{ "../resources/shaders/light.frag" };
  std::filesystem::path light_vertex_shader{ "../resources/shaders/light.vert" };

  Shader model_shader{ model_vertex_shader, model_fragment_shader };
  Shader light_shader{ light_vertex_shader, light_fragment_shader };

  std::vector<Vertex> model_vertices(
    model_vertices_raw, model_vertices_raw + sizeof(model_vertices_raw) / sizeof(Vertex));
  std::vector<GLuint> model_indices(model_indices_raw, model_indices_raw + sizeof(model_indices_raw) / sizeof(GLuint));
  std::vector<Vertex> light_vertices(
    light_vertices_raw, light_vertices_raw + sizeof(light_vertices_raw) / sizeof(Vertex));
  std::vector<GLuint> light_indices(light_indices_raw, light_indices_raw + sizeof(light_indices_raw) / sizeof(GLuint));

  // create meshes
  Mesh model_mesh(model_vertices, model_indices);
  Mesh light_mesh(light_vertices, light_indices);

  glm::vec4 light_color{ 0.5F, 0.5F, 0.5F, 0.5F };
  glm::vec3 light_position{ 0.5F, 0.5F, 0.5F };
  glm::mat4 light_model = glm::mat4(1.0F);
  light_model = glm::translate(light_model, light_position);

  glm::vec3 object_position{ 0.0F, 0.0F, 0.0F };
  glm::mat4 object_model = glm::mat4(1.0F);
  object_model = glm::translate(object_model, object_position);

  light_shader.activate();
  glUniformMatrix4fv(glGetUniformLocation(light_shader.id, "u_model_matrix"), 1, GL_FALSE, glm::value_ptr(light_model));
  glUniform4f(
    glGetUniformLocation(light_shader.id, "u_light_color"), light_color.r, light_color.g, light_color.b, light_color.a);

  model_shader.activate();
  glUniformMatrix4fv(
    glGetUniformLocation(model_shader.id, "u_model_matrix"), 1, GL_FALSE, glm::value_ptr(object_model));
  glUniform4f(
    glGetUniformLocation(model_shader.id, "u_light_color"), light_color.r, light_color.g, light_color.b, light_color.a);
  glUniform3f(
    glGetUniformLocation(model_shader.id, "u_light_position"), light_position.x, light_position.y, light_position.z);

  glEnable(GL_DEPTH_TEST);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);// wireframe

  // create camera
  Camera camera((int)io.DisplaySize.x, (int)io.DisplaySize.y, glm::vec3(0.0F, 0.0F, 2.0F));

  uint32_t last_frametime;
  uint32_t this_frametime;

  // Main loop
  bool done = false;
  while (!done) {
    last_frametime = this_frametime;
    this_frametime = SDL_GetTicks();

    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your
    // inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite
    // your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or
    // clear/overwrite your copy of the keyboard data. Generally you may always pass all inputs to dear imgui, and hide
    // them from your application based on those two flags.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT) { done = true; }
      if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE
          && event.window.windowID == SDL_GetWindowID(window)) {
        done = true;
      }
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(
      clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    while (SDL_PollEvent(&event)) { camera.handle_inputs(event); }
    camera.update_matrix(45.0F, 0.1F, 100.0F);
    model_mesh.draw(model_shader, camera);
    light_mesh.draw(light_shader, camera);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);

    // fps limiter
    if (this_frametime - last_frametime < 1000 / 60) SDL_Delay(1000 / 60 - this_frametime + last_frametime);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  model_shader.cleanup();
  light_shader.cleanup();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}