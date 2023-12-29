#include <catch2/catch_test_macros.hpp>

#include "test_utilities.hpp"
#include "test_utilities_gfx.hpp"

#include "gfx/elementbuffer.hpp"
#include "gfx/mesh.hpp"
#include "gfx/parsergltf.hpp"
#include "gfx/shader.hpp"
#include "gfx/vertexarray.hpp"
#include "gfx/vertexbuffer.hpp"

// test cases
TEST_CASE("ElementBuffer valid creation and cleanup", "[ElementBuffer, EBO]")
{
  using namespace mve::gfx;

  auto [p_window, context]{ create_dummy_opengl_context() };

  std::vector<GLuint> indices;
  ElementBuffer element_buffer{ indices };
  element_buffer.bind();

  REQUIRE(glIsBuffer(element_buffer.id));

  element_buffer.unbind();
  element_buffer.cleanup();

  REQUIRE_FALSE(glIsBuffer(element_buffer.id));

  cleanup_opengl_context(p_window, context);
}

TEST_CASE("VertexBuffer valud creation and cleanup", "[VertexBuffer, VBO]")
{
  using namespace mve::gfx;

  auto [p_window, context]{ create_dummy_opengl_context() };

  std::vector<Vertex> vertices;
  VertexBuffer vertex_buffer{ vertices };

  REQUIRE(glIsBuffer(vertex_buffer.id));

  vertex_buffer.unbind();
  vertex_buffer.cleanup();

  REQUIRE_FALSE(glIsBuffer(vertex_buffer.id));

  cleanup_opengl_context(p_window, context);
}

TEST_CASE("VertexArray valid creation and cleanup", "[VertexArray, VAO]")
{
  using namespace mve::gfx;

  auto [p_window, context]{ create_dummy_opengl_context() };

  VertexArray vertex_array;
  vertex_array.bind();

  REQUIRE(glIsVertexArray(vertex_array.id));

  vertex_array.unbind();
  vertex_array.cleanup();

  REQUIRE_FALSE(glIsVertexArray(vertex_array.id));

  cleanup_opengl_context(p_window, context);
}

TEST_CASE("Empty Shader valid creation and cleanup", "[Shader]")
{
  using namespace mve::gfx;

  std::filesystem::path vertex_shader{ "/workspaces/molviz/test/resources/shaders/empty.vert" };
  std::filesystem::path fragment_shader{ "/workspaces/molviz/test/resources/shaders/empty.frag" };

  auto [p_window, context]{ create_dummy_opengl_context() };

  Shader shader{ vertex_shader, fragment_shader };

  REQUIRE(glIsProgram(shader.id));

  shader.cleanup();

  REQUIRE_FALSE(glIsProgram(shader.id));

  cleanup_opengl_context(p_window, context);
}

TEST_CASE("Default Shader valid creation and cleanup", "[Shader]")
{
  using namespace mve::gfx;

  std::filesystem::path vertex_shader{ "/workspaces/molviz/app/resources/shaders/default.vert" };
  std::filesystem::path fragment_shader{ "/workspaces/molviz/app/resources/shaders/default.frag" };

  auto [p_window, context]{ create_dummy_opengl_context() };

  Shader shader{ vertex_shader, fragment_shader };

  REQUIRE(glIsProgram(shader.id));

  shader.cleanup();

  REQUIRE_FALSE(glIsProgram(shader.id));

  cleanup_opengl_context(p_window, context);
}

TEST_CASE("Invalid vertex shader compile error", "[Shader]")
{
  using namespace mve::gfx;

  std::filesystem::path vertex_shader{ "/workspaces/molviz/test/resources/shaders/invalid.vert" };
  std::filesystem::path fragment_shader{ "/workspaces/molviz/test/resources/shaders/empty.frag" };

  auto [p_window, context]{ create_dummy_opengl_context() };

  REQUIRE_THROWS(Shader(vertex_shader, fragment_shader));

  cleanup_opengl_context(p_window, context);
}

TEST_CASE("Invalid fragment shader compile error", "[Shader]")
{
  using namespace mve::gfx;

  std::filesystem::path vertex_shader{ "/workspaces/molviz/test/resources/shaders/empty.vert" };
  std::filesystem::path fragment_shader{ "/workspaces/molviz/test/resources/shaders/invalid.frag" };

  auto [p_window, context]{ create_dummy_opengl_context() };

  REQUIRE_THROWS(Shader(vertex_shader, fragment_shader));

  cleanup_opengl_context(p_window, context);
}

TEST_CASE("Mesh valid creation and cleanup", "[Mesh]")
{
  using namespace mve::gfx;

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

  cleanup_opengl_context(p_window, context);
}

TEST_CASE("GLTF files valid parsing", "[ParserGLTF, GLTF]")
{
  using namespace mve::gfx;

  auto [p_window, context]{ create_dummy_opengl_context() };

  REQUIRE_NOTHROW(ParserGLTF("/workspaces/molviz/test/resources/meshes/cube_color/cube_color.gltf"));

  cleanup_opengl_context(p_window, context);
}

TEST_CASE("Building valid Model object from GLTF files", "[Model, ParserGLTF, GLTF]")
{
  using namespace mve::gfx;

  auto [p_window, context]{ create_dummy_opengl_context() };

  auto parser{ ParserGLTF("/workspaces/molviz/test/resources/meshes/cube_color/cube_color.gltf") };

  REQUIRE_NOTHROW(parser.get_model());

  cleanup_opengl_context(p_window, context);
}