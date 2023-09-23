#include "gfx/elementbuffer.hpp"

using namespace Molviz::gfx;

ElementBuffer::ElementBuffer(GLuint *tp_indices, GLsizeiptr t_size)
{
  glGenBuffers(1, &id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, t_size, tp_indices, GL_STATIC_DRAW);
};

ElementBuffer::~ElementBuffer() { cleanup(); }

void ElementBuffer::bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }

void ElementBuffer::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void ElementBuffer::cleanup() { glDeleteBuffers(1, &id); }