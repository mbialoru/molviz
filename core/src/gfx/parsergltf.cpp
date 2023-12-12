#include "gfx/parsergltf.hpp"

using namespace Molviz::gfx;

ParserGLTF::ParserGLTF(const char *tp_file) : ParserGLTF(std::filesystem::path(std::string(tp_file))){};

ParserGLTF::ParserGLTF(const std::string &tr_file) : ParserGLTF(std::filesystem::path(tr_file)){};

ParserGLTF::ParserGLTF(const std::filesystem::path &tr_path) : m_path(tr_path) { parse(m_path); };

void ParserGLTF::parse(const char *tp_file) { parse(std::filesystem::path(std::string(tp_file))); }

void ParserGLTF::parse(const std::string &tr_file) { parse(std::filesystem::path(tr_file)); }

void ParserGLTF::parse(const std::filesystem::path &tr_file)
{
  spdlog::debug("loading model file {}", m_path.filename());

  std::string json_data{ file_contents_to_string(tr_file) };

  m_json = nlohmann::json::parse(json_data);
  m_path = tr_file;
  m_data = load_bin_file();

  cleanup();
  process_node(0);
}

Model ParserGLTF::get_model() { return Model(m_meshes, m_translations, m_scales, m_rotations, m_matrices); }

std::vector<uint8_t> ParserGLTF::load_bin_file()
{
  std::string uri = m_json["buffers"][0]["uri"];
  std::filesystem::path parent{ m_path.parent_path() };
  std::string bytes_text{ file_contents_to_string(parent / uri) };
  std::vector<uint8_t> data{ bytes_text.begin(), bytes_text.end() };

  return data;
};

std::vector<Vertex> ParserGLTF::assemble_vertices(std::vector<glm::vec3> t_positions,
  std::vector<glm::vec3> t_normals,
  std::vector<glm::vec4> t_colors)
{
  std::vector<Vertex> vertices;

  for (std::size_t i{ 0 }; i < t_positions.size(); ++i) {
    vertices.emplace_back(t_positions[i], t_normals[i], glm::vec3(1.0F, 1.0F, 1.0F));
  }

  spdlog::debug("loaded {} vertices", vertices.size());

  return vertices;
}

void ParserGLTF::process_node(const std::size_t t_next_node, const glm::mat4 t_matrix)
{
  // current node
  //!! WARNING: do not use curly brace initialization with json lib types, internal types mismatch shenanigans
  nlohmann::json node = m_json["nodes"][t_next_node];

  spdlog::debug("processing node {}", t_next_node);

  // translation if exists
  glm::vec3 node_translation{ glm::vec3(0.0F, 0.0F, 0.0F) };
  if (node.find("translation") != node.end()) {
    spdlog::debug("node has translation");
    float translation_values[3];
    for (std::size_t i{ 0 }; i < node["translation"].size(); ++i) { translation_values[i] = (node["translation"][i]); }
    node_translation = glm::make_vec3(translation_values);
  }

  // rotation if exists
  glm::quat node_rotation{ glm::quat(1.0F, 0.0F, 0.0F, 0.0F) };
  if (node.find("rotation") != node.end()) {
    spdlog::debug("node has rotation");
    float rotation_values[4] = { node["rotation"][3], node["rotation"][0], node["rotation"][1], node["rotation"][2] };
    node_rotation = glm::make_quat(rotation_values);
  }

  // scale if exists
  glm::vec3 node_scale{ glm::vec3(1.0F, 1.0F, 1.0F) };
  if (node.find("scale") != node.end()) {
    spdlog::debug("node has scaling");
    float scale_values[3];
    for (std::size_t i{ 0 }; i < node["scale"].size(); ++i) { scale_values[i] = (node["scale"][i]); }
    node_scale = glm::make_vec3(scale_values);
  }

  // matrix if exists
  glm::mat4 node_matrix{ glm::mat4(1.0F) };
  if (node.find("matrix") != node.end()) {
    spdlog::debug("node has matrix");
    float matrix_values[16];
    for (std::size_t i{ 0 }; i < node["matrix"].size(); ++i) { matrix_values[i] = (node["matrix"][i]); }
    node_matrix = glm::make_mat4(matrix_values);
  }

  glm::mat4 translation{ glm::translate(glm::mat4(1.0F), node_translation) };
  glm::mat4 rotation{ glm::mat4_cast(node_rotation) };
  glm::mat4 scale{ glm::scale(glm::mat4(1.0F), node_scale) };
  glm::mat4 multiplied{ t_matrix * node_matrix * translation * rotation * scale };

  // if node constains mesh - load it
  if (node.find("mesh") != node.end()) {
    spdlog::debug("node has mesh");

    m_translations.push_back(node_translation);
    m_rotations.push_back(node_rotation);
    m_scales.push_back(node_scale);
    m_matrices.push_back(multiplied);

    load_mesh(node["mesh"]);
  }

  // if node has children - iterate over them
  if (node.find("children") != node.end()) {
    for (std::size_t i{ 0 }; i < node["children"].size(); ++i) { process_node(node["children"][i], multiplied); }
  }
}

void ParserGLTF::load_mesh(const std::size_t t_mesh_index)
{
  //!! WARNING: do not use curly brace initialization with json lib types, internal types mismatch shenanigans
  // TODO: Color is not always available, we have to handle when it's not.
  std::size_t color_access_index = m_json["meshes"][t_mesh_index]["primitives"][0]["attributes"]["COLOR_0"];
  std::size_t position_access_index = m_json["meshes"][t_mesh_index]["primitives"][0]["attributes"]["POSITION"];
  std::size_t normal_access_index = m_json["meshes"][t_mesh_index]["primitives"][0]["attributes"]["NORMAL"];
  std::size_t index_access_index = m_json["meshes"][t_mesh_index]["primitives"][0]["indices"];

  // NOTE: color data is type 5123 (uint16_t), we need float 0.0 - 1.0
  std::vector<uint8_t> color_bytes{ get_accessor_data(m_json["accessors"][color_access_index]) };
  std::vector<uint16_t> color_vector{ convert_bytes<uint16_t>(color_bytes) };
  std::vector<float> color_floats;
  std::transform(color_bytes.begin(),
    color_bytes.end(),
    std::back_inserter(color_floats),
    map_range_to_floating_point<uint16_t, float>());
  std::vector<glm::vec4> colors{ group_into_vec4<float>(color_floats) };

  std::vector<uint8_t> position_bytes{ get_accessor_data(m_json["accessors"][position_access_index]) };
  std::vector<float> position_vector{ convert_bytes<float>(position_bytes) };
  std::vector<glm::vec3> positions{ group_into_vec3<float>(position_vector) };

  std::vector<uint8_t> normal_bytes{ get_accessor_data(m_json["accessors"][normal_access_index]) };
  std::vector<float> normal_vector{ convert_bytes<float>(normal_bytes) };
  std::vector<glm::vec3> normals{ group_into_vec3<float>(normal_vector) };

  std::vector<Vertex> vertices{ assemble_vertices(positions, normals, colors) };

  std::vector<uint8_t> indice_bytes{ get_accessor_data(m_json["accessors"][index_access_index]) };
  std::vector<GLuint> indices{ convert_bytes<GLuint, uint16_t>(indice_bytes) };

  m_meshes.push_back(Mesh(vertices, indices));

  spdlog::debug("loaded mesh {}", t_mesh_index);
}

void ParserGLTF::cleanup()
{
  // __force__ dealocation
  if (not m_meshes.empty()) { std::vector<Mesh>().swap(m_meshes); }
  if (not m_translations.empty()) { std::vector<glm::vec3>().swap(m_translations); }
  if (not m_scales.empty()) { std::vector<glm::vec3>().swap(m_scales); }
  if (not m_rotations.empty()) { std::vector<glm::quat>().swap(m_rotations); }
  if (not m_matrices.empty()) { std::vector<glm::mat4>().swap(m_matrices); }
}

std::vector<uint8_t> ParserGLTF::get_accessor_data(nlohmann::json t_accessor)
{
  std::size_t component_type = t_accessor["componentType"];

  switch (component_type) {
  case 5120:// CHAR
    spdlog::debug(fmt::format("type is {}, int8_t", component_type));
    return h_get_accessor_data<int8_t>(t_accessor);
    break;

  case 5121:// UNSIGNED CHAR
    spdlog::debug(fmt::format("type is {}, uint8_t", component_type));
    return h_get_accessor_data<uint8_t>(t_accessor);
    break;

  case 5122:// SHORT
    spdlog::debug(fmt::format("type is {}, int16_t", component_type));
    return h_get_accessor_data<int16_t>(t_accessor);
    break;

  case 5123:// UNSIGNED SHORT
    spdlog::debug(fmt::format("type is {}, uint16_t", component_type));
    return h_get_accessor_data<uint16_t>(t_accessor);
    break;

  case 5125:// UNSIGNED INT
    spdlog::debug(fmt::format("type is {}, uint32_t", component_type));
    return h_get_accessor_data<uint32_t>(t_accessor);
    break;

  case 5126:
    spdlog::debug(fmt::format("type is {}, float_t", component_type));
    return h_get_accessor_data<float_t>(t_accessor);
    break;

  default:
    throw RuntimeErrorLogged(fmt::format("unknown component type {}", component_type));
  }
}