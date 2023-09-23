#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#pragma once

#include <GL/glew.h>

namespace Molviz::gfx {

class VertexBuffer
{
public:
  VertexBuffer(GLfloat *tp_vertices, GLsizeiptr t_size);
  ~VertexBuffer();

  void bind();
  void unbind();
  void cleanup();

  GLuint id;

private:
};

}// namespace Molviz::gfx

#endif// VERTEXBUFFER_HPP