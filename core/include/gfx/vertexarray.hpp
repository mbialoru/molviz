#ifndef VERTEXARRAY_HPP
#define VERTEXARRAY_HPP

#pragma once

#include <GL/glew.h>

#include "vertexbuffer.hpp"

namespace mve::gfx {

class VertexArray
{
public:
  explicit VertexArray();
  ~VertexArray();

  void bind();
  void unbind();
  void cleanup();
  void link_attribute(VertexBuffer &t_buffer,
    GLuint t_layout,
    GLuint t_num_components,
    GLenum t_type,
    GLsizeiptr t_stride,
    void *tp_offset);

  GLuint id;
};

// convenient type alias
using VAO = VertexArray;

}// namespace mve::gfx

#endif// VERTEXARRAY_HPP