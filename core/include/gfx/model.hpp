#ifndef MODEL_HPP
#define MODEL_HPP

#pragma once

#include <nlohmann/json.hpp>

#include "mesh.hpp"

namespace Molviz::gfx {

class Model
{
public:
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

};// namespace Molviz::gfx

#endif// MODEL_HPP