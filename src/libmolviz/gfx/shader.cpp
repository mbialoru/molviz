#include "gfx/shader.hpp"

using namespace Molviz::gfx;

std::string Molviz::gfx::shader_type_to_string(const ShaderType t_type)
{
  switch (t_type) {
  case ShaderType::PROGRAM:
    return std::string("PROGRAM");
    break;
  case ShaderType::FRAGMENT:
    return std::string("FRAGMENT");
    break;
  case ShaderType::VERTEX:
    return std::string("VERTEX");
    break;
  }
}

Shader::Shader(const char *tp_vertex_shader, const char *tp_fragment_shader) : id(glCreateProgram())
{
  std::string vertex_source_string{ file_contents_to_string(tp_vertex_shader) };
  std::string fragment_source_string{ file_contents_to_string(tp_fragment_shader) };

  const char *p_vertex_source{ vertex_source_string.c_str() };
  const char *p_fragment_source{ fragment_source_string.c_str() };

  // process vertex shader
  GLuint vertex_shader{ glCreateShader(GL_VERTEX_SHADER) };
  glShaderSource(vertex_shader, 1, &p_vertex_source, nullptr);
  glCompileShader(vertex_shader);
  compile_errors(vertex_shader, ShaderType::VERTEX);

  // process fragent shader
  GLuint fragment_shader{ glCreateShader(GL_FRAGMENT_SHADER) };
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
};

Shader::~Shader() { cleanup(); }

void Shader::activate() { glUseProgram(id); }

void Shader::cleanup() { glDeleteProgram(id); }

void Shader::compile_errors(GLuint t_shader, const ShaderType t_type)
{
  static const std::size_t BUFFER_SIZE{ 1024 };
  GLint has_compiled;
  char message_buffer[BUFFER_SIZE];

  if (t_type != ShaderType::PROGRAM) {
    glGetShaderiv(t_shader, GL_COMPILE_STATUS, &has_compiled);
    if (has_compiled == GL_FALSE) {
      glGetShaderInfoLog(t_shader, BUFFER_SIZE, NULL, message_buffer);
      spdlog::error("Shader compilation error for: {} {}", shader_type_to_string(t_type), message_buffer);
    }
  } else {
    glGetProgramiv(t_shader, GL_LINK_STATUS, &has_compiled);
    if (has_compiled == GL_FALSE) {
      glGetProgramInfoLog(t_shader, BUFFER_SIZE, NULL, message_buffer);
      spdlog::error("Shader linking error for {} {}", shader_type_to_string(t_type), message_buffer);
    }
  }
}