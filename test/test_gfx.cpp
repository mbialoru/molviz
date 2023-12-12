#include <GL/glew.h>
#include <SDL.h>
#include <catch2/catch_test_macros.hpp>
#include <spdlog/spdlog.h>

#include "gfx/elementbuffer.hpp"
#include "gfx/mesh.hpp"
#include "gfx/model.hpp"
#include "gfx/shader.hpp"
#include "gfx/vertexarray.hpp"
#include "gfx/vertexbuffer.hpp"

// utilities
std::pair<SDL_Window *, SDL_GLContext> create_dummy_opengl_context()
{
  // init SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    spdlog::error("SDL initialization error: {}", SDL_GetError());
    FAIL();
  }

  // create dummy hidden SDL window for a dummy OpenGL context
  SDL_WindowFlags window_flags{ static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN) };
  SDL_Window *p_window{ SDL_CreateWindow(nullptr, 0, 0, 0, 0, window_flags) };
  SDL_GLContext context{ SDL_GL_CreateContext(p_window) };

  // init GLEW after OpenGL context - Missing GL version error otherwise
  GLenum status{ glewInit() };
  if (status != GLEW_OK) {
    std::string error_message{ reinterpret_cast<const char *>(glewGetErrorString(status)) };
    spdlog::error("GLEW init failed! error: {}", error_message);
    FAIL();
  }

  return { p_window, context };
}

void cleanup_dummy_opengl_context(SDL_Window *tp_window, SDL_GLContext t_context)
{
  SDL_GL_DeleteContext(t_context);
  SDL_DestroyWindow(tp_window);
  SDL_Quit();
}

// test cases
TEST_CASE("ElementBuffer valid creation and cleanup", "[ElementBuffer, EBO]")
{
  using namespace Molviz::gfx;

  auto [p_window, context]{ create_dummy_opengl_context() };

  std::vector<GLuint> indices;
  ElementBuffer element_buffer{ indices };
  element_buffer.bind();

  REQUIRE(glIsBuffer(element_buffer.id));

  element_buffer.unbind();
  element_buffer.cleanup();

  REQUIRE_FALSE(glIsBuffer(element_buffer.id));

  cleanup_dummy_opengl_context(p_window, context);
}

TEST_CASE("VertexBuffer valud creation and cleanup", "[VertexBuffer, VBO]")
{
  using namespace Molviz::gfx;

  auto [p_window, context]{ create_dummy_opengl_context() };

  std::vector<Vertex> vertices;
  VertexBuffer vertex_buffer{ vertices };

  REQUIRE(glIsBuffer(vertex_buffer.id));

  vertex_buffer.unbind();
  vertex_buffer.cleanup();

  REQUIRE_FALSE(glIsBuffer(vertex_buffer.id));

  cleanup_dummy_opengl_context(p_window, context);
}

TEST_CASE("VertexArray valid creation and cleanup", "[VertexArray, VAO]")
{
  using namespace Molviz::gfx;

  auto [p_window, context]{ create_dummy_opengl_context() };

  VertexArray vertex_array;
  vertex_array.bind();

  REQUIRE(glIsVertexArray(vertex_array.id));

  vertex_array.unbind();
  vertex_array.cleanup();

  REQUIRE_FALSE(glIsVertexArray(vertex_array.id));

  cleanup_dummy_opengl_context(p_window, context);
}

TEST_CASE("Shader valid creation and cleanup", "[Shader]")
{
  using namespace Molviz::gfx;

  std::filesystem::path vertex_shader{ "/workspaces/molviz/test/resources/shaders/empty.vert" };
  std::filesystem::path fragment_shader{ "/workspaces/molviz/test/resources/shaders/empty.frag" };

  auto [p_window, context]{ create_dummy_opengl_context() };

  Shader shader{ vertex_shader, fragment_shader };

  REQUIRE(glIsProgram(shader.id));

  shader.cleanup();

  REQUIRE_FALSE(glIsProgram(shader.id));

  cleanup_dummy_opengl_context(p_window, context);
}

TEST_CASE("Mesh valid creation and cleanup", "[Mesh]")
{
  using namespace Molviz::gfx;

  std::vector<Vertex> vertices{ Vertex{ glm::vec3(-0.1F, -0.1F, 0.1F) },
    Vertex{ glm::vec3(-0.1F, -0.1F, -0.1F) },
    Vertex{ glm::vec3(0.1F, -0.1F, -0.1F) },
    Vertex{ glm::vec3(0.1F, -0.1F, 0.1F) },
    Vertex{ glm::vec3(-0.1F, 0.1F, 0.1F) },
    Vertex{ glm::vec3(-0.1F, 0.1F, -0.1F) },
    Vertex{ glm::vec3(0.1F, 0.1F, -0.1F) },
    Vertex{ glm::vec3(0.1F, 0.1F, 0.1F) } };

  std::vector<GLuint> indices{
    0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 7, 3, 3, 7, 6, 3, 6, 2, 2, 6, 5, 2, 5, 1, 1, 5, 4, 1, 4, 0, 4, 5, 6, 4, 6, 7
  };

  auto [p_window, context]{ create_dummy_opengl_context() };

  REQUIRE_NOTHROW(Mesh(vertices, indices));
}

TEST_CASE("Model valid creation and cleanup", "[Model]")
{
  using namespace Molviz::gfx;

  auto [p_window, context]{ create_dummy_opengl_context() };

  REQUIRE_NOTHROW(Model("/workspaces/molviz/test/resources/meshes/cube_color/cube_color.gltf"));

  cleanup_dummy_opengl_context(p_window, context);
}