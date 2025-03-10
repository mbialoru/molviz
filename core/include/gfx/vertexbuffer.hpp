#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace mve::gfx {

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 color;
};

class VertexBuffer
{
public:
  explicit VertexBuffer(std::vector<Vertex> &tr_vertices);
  ~VertexBuffer();

  void bind();
  void unbind();
  void cleanup();

  GLuint id;
};

// convenient type alias
using VBO = VertexBuffer;

}// namespace mve::gfx

#endif// VERTEXBUFFER_HPP