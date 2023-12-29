#include "gfx/model.hpp"

using namespace mve::gfx;

Model::Model(ModelData t_data) { std::make_from_tuple<Model>(t_data); }

Model::Model(const std::vector<Mesh> &tr_meshes,
  const std::vector<glm::vec3> &tr_translations,
  const std::vector<glm::vec3> &tr_scales,
  const std::vector<glm::quat> &tr_rotations,
  const std::vector<glm::mat4> &tr_matrices)
  : m_meshes(tr_meshes), m_translations(tr_translations), m_scales(tr_scales), m_rotations(tr_rotations),
    m_matrices(tr_matrices)
{}

void Model::draw(Shader &tr_shader, Camera &tr_camera)
{
  for (std::size_t i{ 0 }; i < m_meshes.size(); ++i) {
    m_meshes[i].draw(tr_shader, tr_camera, m_matrices[i], m_translations[i], m_rotations[i], m_scales[i]);
  }
}