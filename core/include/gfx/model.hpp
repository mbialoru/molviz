#ifndef MODEL_HPP
#define MODEL_HPP

#pragma once

#include "mesh.hpp"

namespace mve::gfx {

typedef std::tuple<std::vector<Mesh> &,
  std::vector<glm::vec3> &,
  std::vector<glm::vec3> &,
  std::vector<glm::quat> &,
  std::vector<glm::mat4> &>
  ModelData;

class Model
{
public:
  Model(ModelData t_data);
  Model(const std::vector<Mesh> &tr_meshes,
    const std::vector<glm::vec3> &tr_translations,
    const std::vector<glm::vec3> &tr_scales,
    const std::vector<glm::quat> &tr_rotations,
    const std::vector<glm::mat4> &tr_matrices);
  ~Model() = default;

  void draw(Shader &tr_shader, Camera &tr_camera);

private:
  std::vector<Mesh> m_meshes;
  std::vector<glm::vec3> m_translations;
  std::vector<glm::vec3> m_scales;
  std::vector<glm::quat> m_rotations;
  std::vector<glm::mat4> m_matrices;
};

};// namespace mve::gfx

#endif// MODEL_HPP