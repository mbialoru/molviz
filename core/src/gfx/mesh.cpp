#include "gfx/mesh.hpp"

using namespace Molviz::gfx;

Mesh::Mesh(const std::vector<Vertex> &tr_vertices, const std::vector<GLuint> &tr_indices)
  : m_vertices(tr_vertices), m_indices(tr_indices), mp_vertex_array(std::make_shared<VertexArray>())
{
  mp_vertex_array->bind();

  mp_vertex_buffer = std::make_shared<VertexBuffer>(m_vertices);
  mp_element_buffer = std::make_shared<ElementBuffer>(m_indices);

  mp_vertex_array->link_attribute(*mp_vertex_buffer, 0, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void *>(0));
  mp_vertex_array->link_attribute(
    *mp_vertex_buffer, 1, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void *>(3 * sizeof(float)));
  mp_vertex_array->link_attribute(
    *mp_vertex_buffer, 2, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void *>(6 * sizeof(float)));

  mp_vertex_array->unbind();
  mp_vertex_buffer->unbind();
  mp_element_buffer->unbind();
};

void Mesh::draw(Shader &tr_shader,
  Camera &tr_camera,
  glm::mat4 t_matrix,
  glm::vec3 t_translation,
  glm::quat t_rotation,
  glm::vec3 t_scale) const
{
  tr_shader.activate();
  mp_vertex_array->bind();

  // handle camera matrix
  glUniform3f(glGetUniformLocation(tr_shader.id, "u_camera_position"),
    tr_camera.position.x,
    tr_camera.position.y,
    tr_camera.position.z);

  tr_camera.export_matrix(tr_shader, "u_camera_matrix");

  // handle transformations
  glm::mat4 translation{ glm::mat4(1.0F) };
  glm::mat4 rotation{ glm::mat4(1.0F) };
  glm::mat4 scale{ glm::mat4(1.0F) };

  translation = glm::translate(translation, t_translation);
  rotation = glm::mat4_cast(t_rotation);
  scale = glm::scale(scale, t_scale);

  glUniformMatrix4fv(glGetUniformLocation(tr_shader.id, "u_translation"), 1, GL_FALSE, glm::value_ptr(translation));
  glUniformMatrix4fv(glGetUniformLocation(tr_shader.id, "u_rotation"), 1, GL_FALSE, glm::value_ptr(rotation));
  glUniformMatrix4fv(glGetUniformLocation(tr_shader.id, "u_scale"), 1, GL_FALSE, glm::value_ptr(scale));
  glUniformMatrix4fv(glGetUniformLocation(tr_shader.id, "u_model_matrix"), 1, GL_FALSE, glm::value_ptr(t_matrix));

  // draw mesh
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);

  mp_vertex_array->unbind();
}