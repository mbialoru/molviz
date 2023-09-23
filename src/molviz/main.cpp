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

#include "gfx/elementbuffer.hpp"
#include "gfx/shader.hpp"
#include "gfx/vertexarray.hpp"
#include "gfx/vertexbuffer.hpp"

// Main code
int main(int argc, char **argv)
{
  // Setup SDL
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
  SDL_Window *window = SDL_CreateWindow(
    "Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, window_flags);
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  glewInit();
  SDL_GL_MakeCurrent(window, gl_context);

  // Use Vsync
  if (SDL_GL_SetSwapInterval(1) < 0) { spdlog::warn("Unable to set VSync! SDL Error: {}", SDL_GetError()); }

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
  ImVec4 clear_color = ImVec4(0.30F, 0.50F, 0.40F, 1.00F);

  // Vertices coordinates
  GLfloat vertices[] = {
    -0.5F,
    0.0F,
    0.5F,
    0.83F,
    0.70F,
    0.44F,
    -0.5F,
    0.0F,
    -0.5F,
    0.83F,
    0.70F,
    0.44F,
    0.5F,
    0.0F,
    -0.5F,
    0.83F,
    0.70F,
    0.44F,
    0.5F,
    0.0F,
    0.5F,
    0.83F,
    0.70F,
    0.44F,
    0.0F,
    0.8F,
    0.0F,
    0.92F,
    0.86F,
    0.76F,
  };

  // Indices for vertices order
  GLuint indices[] = { 0, 1, 2, 0, 2, 3, 0, 1, 4, 1, 2, 4, 2, 3, 4, 3, 0, 4 };

  std::filesystem::path fragment_shader{ "../resources/shaders/default.frag" };
  std::filesystem::path vertex_shader{ "../resources/shaders/default.vert" };

  Molviz::gfx::Shader shader_program{ vertex_shader.c_str(), fragment_shader.c_str() };

  Molviz::gfx::VertexArray VAO;
  VAO.bind();

  Molviz::gfx::VertexBuffer VBO{ vertices, sizeof(vertices) };
  Molviz::gfx::ElementBuffer EBO{ indices, sizeof(indices) };

  VAO.link_attribute(VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);// position
  VAO.link_attribute(VBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));// color

  VAO.unbind();
  VBO.unbind();
  EBO.unbind();

  int uni_id{ glGetUniformLocation(shader_program.id, "scale") };

  // rotation
  float rotation{ 0.0F };
  Uint64 now{ SDL_GetPerformanceCounter() };
  Uint64 last{ 0 };
  double delta_time{ 0.0 };

  glEnable(GL_DEPTH_TEST);

  // Main loop
  bool done = false;
  while (!done) {
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
    shader_program.activate();

    glm::mat4 model{ glm::mat4(1.0F) };
    glm::mat4 view{ glm::mat4(1.0F) };
    glm::mat4 proj{ glm::mat4(1.0F) };

    // rotation
    last = now;
    now = SDL_GetPerformanceCounter();
    delta_time = double((now - last) * 1000) / double(SDL_GetPerformanceFrequency());
    if (delta_time >= (1 / 60 * 1000)) { rotation += 0.5F; }

    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0F, 1.0F, 0.0F));
    view = glm::translate(view, glm::vec3(0.0f, -0.5F, -2.0F));
    proj = glm::perspective(glm::radians(45.0F), float(io.DisplaySize.x / io.DisplaySize.y), 0.1F, 100.0F);

    int model_location{ glGetUniformLocation(shader_program.id, "model") };
    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
    int view_location{ glGetUniformLocation(shader_program.id, "view") };
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
    int proj_location{ glGetUniformLocation(shader_program.id, "proj") };
    glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj));

    glUniform1f(uni_id, 0.5F);
    VAO.bind();
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  VAO.cleanup();
  VBO.cleanup();
  EBO.cleanup();
  shader_program.cleanup();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}