#ifndef ELEMENTBUFFER_HPP
#define ELEMENTBUFFER_HPP

#pragma once

#include <GL/glew.h>

namespace Molviz::gfx {

class ElementBuffer
{
public:
  ElementBuffer(GLuint *tp_indices, GLsizeiptr t_size);
  ~ElementBuffer();

  void bind();
  void unbind();
  void cleanup();

  GLuint id;
};

}// namespace Molviz::gfx

#endif// ELEMENTBUFFER_HPP