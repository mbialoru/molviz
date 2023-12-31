#include "gfx/model.hpp"

using namespace Molviz::gfx;

// TODO: refactor with tests
// TODO: unsigned int to std::size_t

Model::Model(const char *tp_file)
{
  // std::string file_data{ file_contents_to_string(tp_file) };

  // m_json = nlohmann::json::parse(file_data);
  // mp_file = tp_file;
  // m_data = get_data();

  // spdlog::debug("loading model file {}", tp_file);

  // traverse_node(0);

  // NOTE: We're getting somewhere - this simple mesh crashes the program
  // NOTE: default initialized vertices normals and colors do not help
  // NOTE: problem seems not to be with how we acquire data then
  // NOTE: but with how we handle meshes here * confused_af.jpg *

  std::vector<Vertex> vertices{
    Vertex{ glm::vec3(-0.1F, -0.1F, 0.1F), glm::vec3(-0.1F, -0.1F, 0.1F), glm::vec3(0.1F, 0.0F, 0.0F) },
    Vertex{ glm::vec3(-0.1F, -0.1F, -0.1F), glm::vec3(-0.1F, -0.1F, -0.1F), glm::vec3(0.0F, 0.2F, 0.2F) },
    Vertex{ glm::vec3(0.1F, -0.1F, -0.1F), glm::vec3(0.1F, -0.1F, -0.1F), glm::vec3(0.0F, 0.0F, 0.3F) },
    Vertex{ glm::vec3(0.1F, -0.1F, 0.1F), glm::vec3(0.1F, -0.1F, 0.1F), glm::vec3(0.4F, 0.0F, 0.0F) },
    Vertex{ glm::vec3(-0.1F, 0.1F, 0.1F), glm::vec3(-0.1F, 0.1F, 0.1F), glm::vec3(0.0F, 0.0F, 0.5F) },
    Vertex{ glm::vec3(-0.1F, 0.1F, -0.1F), glm::vec3(-0.1F, 0.1F, -0.1F), glm::vec3(0.6F, 0.0F, 0.0F) },
    Vertex{ glm::vec3(0.1F, 0.1F, -0.1F), glm::vec3(0.1F, 0.1F, -0.1F), glm::vec3(0.0F, 0.7F, 0.0F) },
    Vertex{ glm::vec3(0.1F, 0.1F, 0.1F), glm::vec3(0.1F, 0.1F, 0.1F), glm::vec3(0.0F, 0.0F, 0.8F) }
  };

  std::vector<GLuint> indices{
    0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 7, 3, 3, 7, 6, 3, 6, 2, 2, 6, 5, 2, 5, 1, 1, 5, 4, 1, 4, 0, 4, 5, 6, 4, 6, 7
  };

  m_meshes.push_back(Mesh(vertices, indices));
}

void Model::draw(Shader &tr_shader, Camera &tr_camera)
{
  for (std::size_t i{ 0 }; i < m_meshes.size(); ++i) {
    m_meshes[i].draw(tr_shader, tr_camera /* , m_matrices_meshes[i] */);
  }
}

std::vector<unsigned char> Model::get_data()
{
  std::string bytes_text;
  std::string uri = m_json["buffers"][0]["uri"];

  // TODO: refactor to use std::filesystem
  std::string file_string{ std::string(mp_file) };
  std::string file_directory{ file_string.substr(0, file_string.find_last_of('/') + 1) };

  bytes_text = file_contents_to_string((file_directory + uri).c_str());

  std::vector<unsigned char> data{ bytes_text.begin(), bytes_text.end() };

  return data;
}

std::vector<float> Model::get_floats(nlohmann::json t_accessor)
{
  std::vector<float> float_vector;

  unsigned int buffer_view_index = t_accessor.value("bufferView", 1);
  unsigned int count = t_accessor["count"];
  unsigned int access_byte_offset = t_accessor.value("byteOffset", 0);

  std::string type = t_accessor["type"];

  nlohmann::json buffer_view = m_json["bufferViews"][buffer_view_index];
  unsigned int byte_offset{ buffer_view["byteOffset"] };
  unsigned int number_per_vertex;

  // TODO: refactor to use sizeof
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

  return float_vector;
}

std::vector<GLuint> Model::get_indices(nlohmann::json t_accessor)
{
  std::vector<GLuint> indices;

  unsigned int buffer_view_index = t_accessor.value("bufferView", 0);
  unsigned int count = t_accessor["count"];
  unsigned int access_byte_offset = t_accessor.value("byteOffset", 0);
  unsigned int component_type = t_accessor["componentType"];

  nlohmann::json buffer_view = m_json["bufferViews"][buffer_view_index];
  unsigned int byte_offset = buffer_view["byteOffset"];
  unsigned int data_begin = byte_offset + access_byte_offset;

  if (component_type == 5125) {// UNSIGNED_INT
    for (std::size_t i{ data_begin }; i < byte_offset + access_byte_offset + count * 4; i) {
      unsigned char bytes[] = { m_data[i++], m_data[i++], m_data[i++], m_data[i++] };
      unsigned int value;
      std::memcpy(&value, bytes, sizeof(unsigned int));
      indices.push_back((GLuint)(value));
    }
  } else if (component_type == 5123) {// UNSIGNED_SHORT
    for (std::size_t i{ data_begin }; i < byte_offset + access_byte_offset + count * 2; i) {
      unsigned char bytes[] = { m_data[i++], m_data[i++] };
      unsigned short value;
      std::memcpy(&value, bytes, sizeof(unsigned short));
      indices.push_back((GLuint)(value));
    }
  } else if (component_type == 5122) {// SHORT
    for (std::size_t i{ data_begin }; i < byte_offset + access_byte_offset + count * 2; i) {
      unsigned char bytes[] = { m_data[i++], m_data[i++] };
      short value;
      std::memcpy(&value, bytes, sizeof(short));
      indices.push_back((GLuint)(value));
    }
  }

  spdlog::debug("loaded {} indices", indices.size());

  return indices;
}

std::vector<Vertex> Model::assemble_vertices(std::vector<glm::vec3> t_positions, std::vector<glm::vec3> t_normals)
{
  // TODO: color here is fullbright white, would be nice to affect it
  std::vector<Vertex> vertices;

  for (std::size_t i{ 0 }; i < t_positions.size(); ++i) {
    vertices.push_back(Vertex(t_positions[i], t_normals[i], glm::vec3(1.0F, 1.0F, 1.0F)));
  }

  spdlog::debug("loaded {} vertices", vertices.size());

  return vertices;
}

std::vector<glm::vec2> Model::group_floats_vec2(std::vector<float> t_floats)
{
  std::vector<glm::vec2> vectors;
  if (t_floats.size() % 2 != 0) { spdlog::warn("floats vector size not divisible by 2"); }
  for (std::size_t i{ 0 }; i < t_floats.size(); i) { vectors.push_back(glm::vec2(t_floats[i++], t_floats[i++])); }
  return vectors;
}

std::vector<glm::vec3> Model::group_floats_vec3(std::vector<float> t_floats)
{
  std::vector<glm::vec3> vectors;
  if (t_floats.size() % 3 != 0) { spdlog::warn("floats vector size not divisible by 3"); }
  for (std::size_t i{ 0 }; i < t_floats.size(); i) {
    vectors.push_back(glm::vec3(t_floats[i++], t_floats[i++], t_floats[i++]));
  }
  return vectors;
}

std::vector<glm::vec4> Model::group_floats_vec4(std::vector<float> t_floats)
{
  std::vector<glm::vec4> vectors;
  if (t_floats.size() % 4 != 0) { spdlog::warn("floats vector size not divisible by 4"); }
  for (std::size_t i{ 0 }; i < t_floats.size(); i) {
    vectors.push_back(glm::vec4(t_floats[i++], t_floats[i++], t_floats[i++], t_floats[i++]));
  }
  return vectors;
}

void Model::load_mesh(unsigned int t_mesh_index)
{
  //!! WARNING: do not use curly brace initialization with json, internal types mismatch
  unsigned int position_access_index = m_json["meshes"][t_mesh_index]["primitives"][0]["attributes"]["POSITION"];
  unsigned int normal_access_index = m_json["meshes"][t_mesh_index]["primitives"][0]["attributes"]["NORMAL"];
  unsigned int index_access_index = m_json["meshes"][t_mesh_index]["primitives"][0]["indices"];

  std::vector<float> position_vector{ get_floats(m_json["accessors"][position_access_index]) };
  std::vector<glm::vec3> positions{ group_floats_vec3(position_vector) };

  std::vector<float> normal_vector{ get_floats(m_json["accessors"][normal_access_index]) };
  std::vector<glm::vec3> normals{ group_floats_vec3(normal_vector) };

  std::vector<Vertex> vertices{ assemble_vertices(positions, normals) };
  std::vector<GLuint> indices{ get_indices(m_json["accessors"][index_access_index]) };

  m_meshes.push_back(Mesh(vertices, indices));

  spdlog::debug("loaded mesh {}", t_mesh_index);
}

void Model::traverse_node(unsigned int t_next_node, glm::mat4 t_matrix)
{
  // current node
  //!! WARNING: do not use curly brace initialization, types mismatch
  nlohmann::json node = m_json["nodes"][t_next_node];

  spdlog::debug("processing node {}", t_next_node);

  // translation if exists
  glm::vec3 translation = glm::vec3(0.0F, 0.0F, 0.0F);
  if (node.find("translation") != node.end()) {
    spdlog::debug("node has translation");
    float translation_values[3];
    // TODO: this loop should be replaced with assignment because it can overflow
    for (std::size_t i{ 0 }; i < node["translation"].size(); ++i) { translation_values[i] = (node["translation"][i]); }
    translation = glm::make_vec3(translation_values);
  }

  // rotation if exists
  glm::quat rotation = glm::quat(1.0F, 0.0F, 0.0F, 0.0F);
  if (node.find("rotation") != node.end()) {
    spdlog::debug("node has rotation");
    float rotation_values[4] = { node["rotation"][3], node["rotation"][0], node["rotation"][1], node["rotation"][2] };
    rotation = glm::make_quat(rotation_values);
  }

  // scale if exists
  glm::vec3 scale = glm::vec3(1.0F, 1.0F, 1.0F);
  if (node.find("scale") != node.end()) {
    spdlog::debug("node has scaling");
    float scale_values[3];
    for (std::size_t i{ 0 }; i < node["scale"].size(); ++i) { scale_values[i] = (node["scale"][i]); }
    scale = glm::make_vec3(scale_values);
  }

  // matrix if exists
  glm::mat4 matrix_node = glm::mat4(1.0F);
  if (node.find("matrix") != node.end()) {
    spdlog::debug("node has matrix");
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

  glm::mat4 matrix_next_node = t_matrix * matrix_node * trans * rot * sca;

  // if node constains mesh - load it
  if (node.find("mesh") != node.end()) {
    spdlog::debug("node has mesh");

    m_translation_meshes.push_back(translation);
    m_rotation_meshes.push_back(rotation);
    m_scale_meshes.push_back(scale);
    m_matrices_meshes.push_back(matrix_next_node);

    load_mesh(node["mesh"]);
  }

  // if node has children - iterate over them
  if (node.find("children") != node.end()) {
    for (std::size_t i{ 0 }; i < node["children"].size(); ++i) { traverse_node(node["children"][i], matrix_next_node); }
  }
}