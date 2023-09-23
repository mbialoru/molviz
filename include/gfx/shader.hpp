#ifndef SHADER_HPP
#define SHADER_HPP

#pragma once

#include <GL/glew.h>
#include <spdlog/spdlog.h>

#include "utilities.hpp"

namespace Molviz::gfx {

enum class ShaderType { FRAGMENT, VERTEX, PROGRAM };

std::string shader_type_to_string(const ShaderType t_type);

class Shader
{
public:
  Shader(const char *tp_vertex_shader, const char *tp_fragment_shader);
  Shader(const std::string t_vertex_shader, const std::string t_fragment_shader);
  Shader(const std::filesystem::path t_vertex_shader, const std::filesystem::path t_fragment_shader);
  ~Shader();

  void activate();
  void cleanup();

  GLuint id;

private:
  void compile_errors(GLuint t_shader, const ShaderType t_type);
};

}// namespace Molviz::gfx

#endif// SHADER_HPP