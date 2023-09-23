#include "gfx/vertexbuffer.hpp"

using namespace Molviz::gfx;

VertexBuffer::VertexBuffer(GLfloat *tp_vertices, GLsizeiptr t_size)
{
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, t_size, tp_vertices, GL_STATIC_DRAW);
};

VertexBuffer::~VertexBuffer() { cleanup(); }

void VertexBuffer::bind() { glBindBuffer(GL_ARRAY_BUFFER, id); }

void VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::cleanup() { glDeleteBuffers(1, &id); }