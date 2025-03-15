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
  std::vector<unsigned char> get_data();
  std::vector<float> get_floats(nlohmann::json t_accessor);
  std::vector<GLuint> get_indices(nlohmann::json t_accessor);

  std::vector<Vertex> assemble_vertices(std::vector<glm::vec3> t_positions, std::vector<glm::vec3> t_normals);

  std::vector<glm::vec2> group_floats_vec2(std::vector<float> t_floats);
  std::vector<glm::vec3> group_floats_vec3(std::vector<float> t_floats);
  std::vector<glm::vec4> group_floats_vec4(std::vector<float> t_floats);

  void load_mesh(unsigned int t_mesh_index);
  void traverse_node(unsigned int t_next_node, glm::mat4 t_matrix = glm::mat4(1.0F));

  const char *mp_file;

  std::vector<unsigned char> m_data;

  nlohmann::json m_json;

  std::vector<Mesh> m_meshes;
  std::vector<glm::vec3> m_translation_meshes;
  std::vector<glm::vec3> m_scale_meshes;
  std::vector<glm::quat> m_rotation_meshes;
  std::vector<glm::mat4> m_matrices_meshes;
};

};// namespace Molviz::gfx

#endif// MODEL_HPP