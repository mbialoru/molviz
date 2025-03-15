#include "libmolviz/gfx/vertexarray.hpp"

using namespace Molviz::gfx;

VertexArray::VertexArray() { glGenVertexArrays(1, &id); };

void VertexArray::bind() { glBindVertexArray(id); }

void VertexArray::unbind() { glBindVertexArray(0); }

void VertexArray::cleanup() { glDeleteVertexArrays(1, &id); }

void VertexArray::link_attribute(VertexBuffer &tr_buffer,
  GLuint t_layout,
  GLuint t_num_components,
  GLenum t_type,
  GLsizeiptr t_stride,
  void *tp_offset)
{
  tr_buffer.bind();
  glVertexAttribPointer(t_layout, t_num_components, t_type, GL_FALSE, t_stride, tp_offset);
  glEnableVertexAttribArray(t_layout);
  tr_buffer.unbind();
}