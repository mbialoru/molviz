#ifndef MODEL_HPP
#define MODEL_HPP

#pragma once

#include <nlohmann/json.hpp>

#include "mesh.hpp"

namespace Molviz::gfx {

class Model
{
public:
  Model(const char *tp_file);
  ~Model() = default;

  void draw(Shader &tr_shader, Camera &tr_camera);

private:
  // std::vector<uint8_t> get_data();
  // std::vector<float> get_floats(nlohmann::json t_accessor);
  // std::vector<GLuint> get_indices(nlohmann::json t_accessor);

  // std::vector<Vertex> assemble_vertices(std::vector<glm::vec3> t_positions,
  //   std::vector<glm::vec3> t_normals,
  //   std::vector<glm::vec4> t_colors);

  // void load_mesh(std::size_t t_mesh_index);

  std::vector<Mesh> m_meshes;
  std::vector<glm::vec3> m_meshes_translations;
  std::vector<glm::vec3> m_meshes_scales;
  std::vector<glm::quat> m_meshes_rotations;
  std::vector<glm::mat4> m_meshes_matrices;
};

};// namespace Molviz::gfx

#endif// MODEL_HPP