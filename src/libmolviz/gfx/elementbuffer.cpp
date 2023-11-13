#include "libmolviz/gfx/elementbuffer.hpp"

using namespace Molviz::gfx;

ElementBuffer::ElementBuffer(std::vector<GLuint> &tr_indices)
{
  glGenBuffers(1, &id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, tr_indices.size() * sizeof(GLuint), tr_indices.data(), GL_STATIC_DRAW);
};

void ElementBuffer::bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }

void ElementBuffer::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void ElementBuffer::cleanup() { glDeleteBuffers(1, &id); }