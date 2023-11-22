#include "gfx/mesh.hpp"

using namespace Molviz::gfx;

Mesh::Mesh(std::vector<Vertex> &tr_vertices, std::vector<GLuint> &tr_indices)
  : vertices(tr_vertices), indices(tr_indices)
{
  p_vertex_array = std::make_unique<VertexArray>(VertexArray());
  p_vertex_array->bind();

  p_vertex_buffer = std::make_unique<VertexBuffer>(VertexBuffer(vertices));
  p_element_buffer = std::make_unique<ElementBuffer>(ElementBuffer(indices));

  p_vertex_array->link_attribute(*p_vertex_buffer.get(), 0, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void *>(0));
  p_vertex_array->link_attribute(
    *p_vertex_buffer.get(), 1, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void *>(3 * sizeof(float)));
  p_vertex_array->link_attribute(
    *p_vertex_buffer.get(), 2, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void *>(6 * sizeof(float)));

  p_vertex_array->unbind();
  p_vertex_buffer->unbind();
  p_element_buffer->unbind();
};

Mesh::Mesh(Mesh &&tr_other)
{
  tr_other.p_element_buffer.swap(p_element_buffer);
  tr_other.p_vertex_buffer.swap(p_vertex_buffer);
  tr_other.p_vertex_array.swap(p_vertex_array);
}

Mesh::~Mesh()
{
  if (p_element_buffer) p_element_buffer->cleanup();
  if (p_vertex_buffer) p_vertex_buffer->cleanup();
  if (p_vertex_array) p_vertex_array->cleanup();
}

void Mesh::draw(Shader &tr_shader,
  Camera &tr_camera,
  glm::mat4 t_matrix,
  glm::vec3 t_translation,
  glm::quat t_rotation,
  glm::vec3 t_scale)
{
  tr_shader.activate();
  p_vertex_array->bind();

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
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}