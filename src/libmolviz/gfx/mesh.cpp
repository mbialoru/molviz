#include "gfx/mesh.hpp"

using namespace Molviz::gfx;

Mesh::Mesh(std::vector<Vertex> &tr_vertices, std::vector<GLuint> &tr_indices)
  : vertices(tr_vertices), indices(tr_indices)
{
  vertex_array.bind();

  VertexBuffer vertex_buffer(vertices);
  ElementBuffer element_buffer(indices);

  vertex_array.link_attribute(vertex_buffer, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
  vertex_array.link_attribute(vertex_buffer, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(float)));
  vertex_array.link_attribute(vertex_buffer, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float)));

  vertex_array.unbind();
  vertex_buffer.unbind();
  element_buffer.unbind();
};

void Mesh::draw(Shader &tr_shader, Camera &tr_camera)
{
  tr_shader.activate();
  vertex_array.bind();

  glUniform3f(glGetUniformLocation(tr_shader.id, "u_camera_position"),
    tr_camera.position.x,
    tr_camera.position.y,
    tr_camera.position.z);

  tr_camera.export_matrix(tr_shader, "u_camera_matrix");

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}