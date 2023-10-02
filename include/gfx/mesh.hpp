#ifndef MESH_HPP
#define MESH_HPP

#pragma once

#include <string>
#include <vector>

#include "camera.hpp"
#include "elementbuffer.hpp"
#include "vertexarray.hpp"

namespace Molviz::gfx {

class Mesh
{
public:
  Mesh(std::vector<Vertex> &tr_vertices, std::vector<GLuint> &tr_indices);
  ~Mesh() = default;

  void draw(Shader &tr_shader, Camera &tr_camera);

  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;

  VertexArray vertex_array;
};

}// namespace Molviz::gfx

#endif// MESH_HPP