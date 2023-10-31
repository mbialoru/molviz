#include "gfx/model.hpp"

using namespace Molviz::gfx;

// TODO: refactor with tests
// TODO: unsigned int to std::size_t

Model::Model(const char *tp_file)
{
  std::string file_data{ file_contents_to_string(tp_file) };

  m_json = nlohmann::json::parse(file_data);
  mp_file = tp_file;
  m_data = get_data();

  traverse_node(0);
}

void Model::draw(Shader &tr_shader, Camera &tr_camera)
{
  for (std::size_t i{ 0 }; i < m_meshes.size(); ++i) {
    m_meshes[i].Mesh::draw(tr_shader, tr_camera, m_matrices_meshes[i]);
  }
}

std::vector<unsigned char> Model::get_data()
{
  std::string bytes_text;
  std::string uri = m_json["buffers"][0]["uri"];

  // NOTE: refactor to use std::filesystem
  std::string file_string{ std::string(mp_file) };
  std::string file_directory{ file_string.substr(0, file_string.find_last_of('/') + 1) };

  bytes_text = file_contents_to_string((file_directory + uri).c_str());

  std::vector<unsigned char> data{ bytes_text.begin(), bytes_text.end() };

  return data;
}

std::vector<float> Model::get_floats(nlohmann::json t_accessor)
{
  std::vector<float> float_vector;

  unsigned int buffer_view_index{ t_accessor.value("bufferView", 1) };
  unsigned int count{ t_accessor["count"] };
  unsigned int access_byte_offset{ t_accessor.value("byteOffset", 0) };

  std::string type{ t_accessor["type"] };

  nlohmann::json buffer_view{ m_json["bufferViews"][buffer_view_index] };
  unsigned int byte_offset{ buffer_view["byteOffset"] };
  unsigned int number_per_vertex;

  // NOTE: refactor to use sizeof
  if (type == "SCALAR")
    number_per_vertex = 1;
  else if (type == "VEC2")
    number_per_vertex = 2;
  else if (type == "VEC3")
    number_per_vertex = 3;
  else if (type == "VEC4")
    number_per_vertex = 4;
  else {
    spdlog::error("type is invalid {} given", type);
    throw std::invalid_argument("type is invalid");
  }

  unsigned int data_begin = byte_offset + access_byte_offset;
  unsigned int data_size = count * 4 * number_per_vertex;

  for (std::size_t i{ data_begin }; i < data_begin + data_size; i) {
    unsigned char bytes[] = { m_data[i++], m_data[i++], m_data[i++], m_data[i++] };
    float value;
    std::memcpy(&value, bytes, sizeof(float));
    float_vector.push_back(value);
  }
}

std::vector<GLuint> Model::get_indices(nlohmann::json t_accessor)
{
  std::vector<GLuint> indices;

  unsigned int buffer_vier_index{ t_accessor.value("bufferView", 0) };
  unsigned int count{ t_accessor["count"] };
  unsigned int access_byte_offset{ t_accessor.value("byteOffset", 0) };
  unsigned int component_type{ t_accessor["componentType"] };

  nlohmann::json buffer_view{ m_json["bufferViews"][buffer_vier_index] };
  unsigned int byte_offset{ buffer_view["byteOffset"] };

  unsigned int data_begin = byte_offset + access_byte_offset;

  if (component_type == 5125) {// UNSIGNED_INT
    for (std::size_t i{ data_begin }; i < byte_offset + access_byte_offset + count * 4; i) {
      unsigned char bytes[] = { m_data[i++], m_data[i++], m_data[i++], m_data[i++] };
      unsigned int value;
      std::memcpy(&value, bytes, sizeof(unsigned int));
      indices.push_back(static_cast<GLuint>(value));
    }
  } else if (component_type == 5123) {// UNSIGNED_SHORT
    for (std::size_t i{ data_begin }; i < byte_offset + access_byte_offset + count * 2; i) {
      unsigned char bytes[] = { m_data[i++], m_data[i++] };
      unsigned short value;
      std::memcpy(&value, bytes, sizeof(unsigned short));
      indices.push_back(static_cast<GLuint>(value));
    }
  } else if (component_type == 5122) {// SHORT
    for (std::size_t i{ data_begin }; i < byte_offset + access_byte_offset + count * 2; i) {
      unsigned char bytes[] = { m_data[i++], m_data[i++] };
      short value;
      std::memcpy(&value, bytes, sizeof(short));
      indices.push_back(static_cast<GLuint>(value));
    }
  }
  return indices;
}

std::vector<Vertex> Model::assemble_vertices(std::vector<glm::vec3> t_positions, std::vector<glm::vec3> t_normals)
{
  // TODO: color here is fullbright white
  std::vector<Vertex> vertices;
  for (std::size_t i{ 0 }; i < t_positions.size(); ++i) {
    vertices.push_back(Vertex(t_positions[i], t_normals[i], glm::vec3(1.0F, 1.0F, 1.0F)));
  }
}

std::vector<glm::vec2> Model::group_floats_vec2(std::vector<float> t_floats)
{
  std::vector<glm::vec2> vectors;
  for (std::size_t i{ 0 }; i < t_floats.size(); i) { vectors.push_back(glm::vec2(t_floats[i++], t_floats[i++])); }
  return vectors;
}

std::vector<glm::vec3> Model::group_floats_vec3(std::vector<float> t_floats)
{
  std::vector<glm::vec3> vectors;
  for (std::size_t i{ 0 }; i < t_floats.size(); i) {
    vectors.push_back(glm::vec3(t_floats[i++], t_floats[i++], t_floats[i++]));
  }
  return vectors;
}

std::vector<glm::vec4> Model::group_floats_vec4(std::vector<float> t_floats)
{
  std::vector<glm::vec4> vectors;
  for (std::size_t i{ 0 }; i < t_floats.size(); i) {
    vectors.push_back(glm::vec4(t_floats[i++], t_floats[i++], t_floats[i++], t_floats[i++]));
  }
  return vectors;
}

void Model::load_mesh(unsigned int t_mesh_index)
{
  unsigned int position_access_index{ m_json["meshes"][t_mesh_index]["primitives"][0]["attributes"]["POSITION"] };
  unsigned int normal_access_index{ m_json["meshes"][t_mesh_index]["primitives"][0]["attributes"]["NORMAL"] };
  unsigned int index_access_index{ m_json["meshes"][t_mesh_index]["primitives"][0]["indices"] };

  std::vector<float> position_vector{ get_floats(m_json["accessors"][position_access_index]) };
  std::vector<glm::vec3> positions{ group_floats_vec3(position_vector) };
  std::vector<float> normal_vector{ get_floats(m_json["accessors"][normal_access_index]) };
  std::vector<glm::vec3> normals{ group_floats_vec3(normal_vector) };

  std::vector<Vertex> vertices{ assemble_vertices(positions, normals) };
  std::vector<GLuint> indices{ get_indices(m_json["accessors"][index_access_index]) };

  m_meshes.push_back(Mesh(vertices, indices));
}

void Model::traverse_node(unsigned int t_next_node, glm::mat4 t_matrix)
{
  nlohmann::json node{ m_json["nodes"][t_next_node] };
  glm::vec3 translation{ glm::vec3(0.0F, 0.0F, 0.0F) };

  if (node.find("translation") != node.end()) {
    // TODO: this loop is pointless and can overflow
    float translation_values[3];
    for (std::size_t i{ 0 }; i < node["translation"].size(); ++i) { translation_values[i] = (node["translation"][i]); }
    translation = glm::make_vec3(translation_values);
  }

  glm::quat rotation{ glm::quat(1.0F, 0.0F, 0.0F, 0.0F) };
  if (node.find("rotation") != node.end()) {
    float rotation_values[] = { node["rotation"][3], node["rotation"][0], node["rotation"][1], node["rotation"][2] };
    rotation = glm::make_quat(rotation_values);
  }

  glm::vec3 scale{ glm::vec3(1.0F, 1.0F, 1.0F) };
  if (node.find("scale") != node.end()) {
    float scale_values[3];
    for (std::size_t i{ 0 }; i < node["scale"].size(); ++i) { scale_values[i] = (node["scale"[i]]); }
    scale = glm::make_vec3(scale_values);
  }

  glm::mat4 matrix_node{ glm::mat4(1.0F) };
  if (node.find("matrix") != node.end()) {
    float matrix_values[16];
    for (std::size_t i{ 0 }; i < node["matrix"].size(); ++i) { matrix_values[i] = (node["matrix"][i]); }
    matrix_node = glm::make_mat4(matrix_values);
  }

  glm::mat4 trans{ glm::mat4(1.0F) };
  glm::mat4 rot{ glm::mat4(1.0F) };
  glm::mat4 sca{ glm::mat4(1.0F) };

  trans = glm::translate(trans, translation);
  rot = glm::mat4_cast(rotation);
  sca = glm::scale(sca, scale);

  glm::mat4 matrix_next_node{ t_matrix * matrix_node * trans * rot * sca };

  if (node.find("mesh") != node.end()) {
    m_translation_meshes.push_back(translation);
    m_rotation_meshes.push_back(rotation);
    m_scale_meshes.push_back(scale);
    m_matrices_meshes.push_back(matrix_next_node);

    load_mesh(node["mesh"]);
  }

  if (node.find("children") != node.end()) {
    for (std::size_t i{ 0 }; i < node["children"].size(); ++i) { traverse_node(node["children"][i], matrix_next_node); }
  }
}