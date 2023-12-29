#ifndef MESH_HPP
#define MESH_HPP

#pragma once

#include <string>
#include <vector>

#include "camera.hpp"
#include "elementbuffer.hpp"
#include "vertexarray.hpp"

namespace mve::gfx {

class Mesh
{
public:
  Mesh(const std::vector<Vertex> &tr_vertices, const std::vector<GLuint> &tr_indices);
  ~Mesh() = default;

  void draw(Shader &tr_shader,
    Camera &tr_camera,
    glm::mat4 t_matrix = glm::mat4(1.0F),
    glm::vec3 t_translation = glm::vec3(0.0F, 0.0F, 0.0F),
    glm::quat t_rotation = glm::quat(1.0F, 0.0F, 0.0F, 0.0F),
    glm::vec3 t_scale = glm::vec3(1.0F, 1.0F, 1.0F)) const;

private:
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  std::shared_ptr<VertexArray> mp_vertex_array;
  std::shared_ptr<VertexBuffer> mp_vertex_buffer;
  std::shared_ptr<ElementBuffer> mp_element_buffer;
};

}// namespace mve::gfx

#endif// MESH_HPP