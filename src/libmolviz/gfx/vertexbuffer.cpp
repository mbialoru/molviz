#include "libmolviz/gfx/vertexbuffer.hpp"

using namespace Molviz::gfx;

VertexBuffer::VertexBuffer(std::vector<Vertex> &tr_vertices)
{
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, tr_vertices.size() * sizeof(Vertex), tr_vertices.data(), GL_STATIC_DRAW);
};

void VertexBuffer::bind() { glBindBuffer(GL_ARRAY_BUFFER, id); }

void VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::cleanup() { glDeleteBuffers(1, &id); }