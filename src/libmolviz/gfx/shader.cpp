#include "gfx/shader.hpp"

using namespace Molviz::gfx;

std::string Molviz::gfx::shader_type_to_string(const ShaderType t_type)
{
  switch (t_type) {
  case ShaderType::PROGRAM:
    return { "PROGRAM" };
    break;
  case ShaderType::FRAGMENT:
    return { "FRAGMENT" };
    break;
  case ShaderType::VERTEX:
    return { "VERTEX" };
    break;
  default:
    spdlog::error("invalid shader type {} given", static_cast<int>(t_type));
    throw std::invalid_argument("invalid shader type");
    break;
  }
}

Shader::Shader(const char *tp_vertex_shader, const char *tp_fragment_shader)
  : Shader(std::filesystem::path(tp_vertex_shader), std::filesystem::path(tp_fragment_shader))
{}

Shader::Shader(const std::string &tr_vertex_shader, const std::string &tr_fragment_shader)
  : Shader(std::filesystem::path(tr_vertex_shader), std::filesystem::path(tr_fragment_shader))
{}

Shader::Shader(const std::filesystem::path &tr_vertex_shader, const std::filesystem::path &tr_fragment_shader)
  : id(glCreateProgram())
{
  const std::string vertex_source_string{ file_contents_to_string(tr_vertex_shader) };
  const std::string fragment_source_string{ file_contents_to_string(tr_fragment_shader) };

  const char *p_vertex_source{ vertex_source_string.c_str() };
  const char *p_fragment_source{ fragment_source_string.c_str() };

  // process vertex shader
  const GLuint vertex_shader{ glCreateShader(GL_VERTEX_SHADER) };
  glShaderSource(vertex_shader, 1, &p_vertex_source, nullptr);
  glCompileShader(vertex_shader);
  compile_errors(vertex_shader, ShaderType::VERTEX);

  // process fragent shader
  const GLuint fragment_shader{ glCreateShader(GL_FRAGMENT_SHADER) };
  glShaderSource(fragment_shader, 1, &p_fragment_source, nullptr);
  glCompileShader(fragment_shader);
  compile_errors(fragment_shader, ShaderType::FRAGMENT);

  // process shader program
  glAttachShader(id, vertex_shader);
  glAttachShader(id, fragment_shader);
  glLinkProgram(id);
  compile_errors(id, ShaderType::PROGRAM);

  // cleanup
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  spdlog::info("initialized shader program");
  spdlog::debug("vertex shader file {}", tr_vertex_shader.filename().string());
  spdlog::debug("fragment shader file {}", tr_fragment_shader.filename().string());
}

Shader::~Shader() { cleanup(); }

void Shader::activate() const { glUseProgram(id); }

void Shader::cleanup() const { glDeleteProgram(id); }

void Shader::compile_errors(GLuint t_shader, const ShaderType t_type)
{
  static const std::size_t BUFFER_SIZE{ 1024 };
  GLint has_compiled{ GL_FALSE };
  std::array<char, BUFFER_SIZE> message_buffer{};

  if (t_type != ShaderType::PROGRAM) {
    glGetShaderiv(t_shader, GL_COMPILE_STATUS, &has_compiled);
    if (has_compiled == GL_FALSE) {
      glGetShaderInfoLog(t_shader, BUFFER_SIZE, nullptr, message_buffer.data());
      std::string message{ message_buffer.begin(), message_buffer.end() };
      spdlog::error("shader compilation error for: {} {}", shader_type_to_string(t_type), message);
    }
  } else {
    glGetProgramiv(t_shader, GL_LINK_STATUS, &has_compiled);
    if (has_compiled == GL_FALSE) {
      glGetProgramInfoLog(t_shader, BUFFER_SIZE, nullptr, message_buffer.data());
      std::string message{ message_buffer.begin(), message_buffer.end() };
      spdlog::error("shader linking error for {} {}", shader_type_to_string(t_type), message);
    }
  }
}