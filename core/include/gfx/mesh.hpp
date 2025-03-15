#ifndef MESH_HPP
#define MESH_HPP

#pragma once

#include <string>
#include <vector>

#include "camera.hpp"
#include "elementbuffer.hpp"
#include "vertexarray.hpp"

namespace Molviz::gfx {

// NOTE: using shared_ptr may have unforeseen consequences when copying
// TODO: in case above is true - implement explicit copy constructor and
// rollback to using unique_ptr. This also would call for removal of destructors
// in VAO, EBO and VAO classes !

class Mesh
{
public:
  Mesh(std::vector<Vertex> &tr_vertices, std::vector<GLuint> &tr_indices);
  ~Mesh() = default;

  void draw(Shader &tr_shader,
    Camera &tr_camera,
    glm::mat4 t_matrix = glm::mat4(1.0F),
    glm::vec3 t_translation = glm::vec3(0.0F, 0.0F, 0.0F),
    glm::quat t_rotation = glm::quat(1.0F, 0.0F, 0.0F, 0.0F),
    glm::vec3 t_scale = glm::vec3(1.0F, 1.0F, 1.0F)) const;

  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;

  std::shared_ptr<VertexArray> p_vertex_array;
  std::shared_ptr<VertexBuffer> p_vertex_buffer;
  std::shared_ptr<ElementBuffer> p_element_buffer;
};

}// namespace Molviz::gfx

#endif// MESH_HPP