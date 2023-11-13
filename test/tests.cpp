#include <GL/glew.h>
#include <SDL.h>
#include <catch2/catch_test_macros.hpp>
#include <spdlog/spdlog.h>

#include "libmolviz/gfx/elementbuffer.hpp"
#include "libmolviz/gfx/vertexarray.hpp"

TEST_CASE("ElementBuffer valid creation and cleanup", "[ElementBuffer, EBO]") {}

TEST_CASE("VertexArray valid creation and cleanup", "[VertexArray, VAO]")
{
  using namespace Molviz::gfx;

  // initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    spdlog::error("SDL initialization error: {}", SDL_GetError());
    FAIL();
  }

  // create dummy SDL window for getting a dummy OpenGL context
  SDL_WindowFlags window_flags{ static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN) };
  SDL_Window *window{ SDL_CreateWindow(nullptr, 0, 0, 0, 0, window_flags) };
  SDL_GLContext gl_context{ SDL_GL_CreateContext(window) };

  // init GLEW
  GLenum status{ glewInit() };
  if (status != GLEW_OK) {
    std::string error_message{ reinterpret_cast<const char *>(glewGetErrorString(status)) };
    spdlog::error("GLEW init failed! error:{}", error_message);
    FAIL();
  }

  // create VAO
  VertexArray vertex_array;
  vertex_array.bind();

  // check if array was reserved
  REQUIRE(glIsVertexArray(vertex_array.id));

  // destroy reserved array
  vertex_array.unbind();
  vertex_array.cleanup();

  // check if array has been destroyed
  REQUIRE(not glIsVertexArray(vertex_array.id));

  // cleanup
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}