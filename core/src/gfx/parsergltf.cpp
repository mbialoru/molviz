#include "gfx/parsergltf.hpp"

using namespace Molviz::gfx;

ParserGLTF::ParserGLTF(const char *tp_file) : ParserGLTF(std::filesystem::path(std::string(tp_file))){};

ParserGLTF::ParserGLTF(const std::string &tr_file) : ParserGLTF(std::filesystem::path(tr_file)){};

ParserGLTF::ParserGLTF(const std::filesystem::path &tr_file) : m_file(tr_file) { parse(m_file); };

void ParserGLTF::parse(const char *tp_file) { parse(std::filesystem::path(std::string(tp_file))); }

void ParserGLTF::parse(const std::string &tr_file) { parse(std::filesystem::path(tr_file)); }

void ParserGLTF::parse(const std::filesystem::path &tr_file)
{
  spdlog::debug("loading model file {}", tr_file.filename().string());

  const std::string json_data{ file_contents_to_string(tr_file) };

  m_json = nlohmann::json::parse(json_data);
  m_file = tr_file;
  m_data = load_bin_file();

  cleanup();
  process_node(0);
}

Model ParserGLTF::get_model() { return { m_meshes, m_translations, m_scales, m_rotations, m_matrices }; }

std::vector<uint8_t> ParserGLTF::load_bin_file()
{
  std::string uri = m_json["buffers"][0]["uri"];
  std::filesystem::path parent{ m_file.parent_path() };
  std::string bytes_text{ file_contents_to_string(parent / uri) };
  std::vector<uint8_t> data{ bytes_text.begin(), bytes_text.end() };

  return data;
};

std::vector<Vertex> ParserGLTF::assemble_vertices(std::vector<glm::vec3> t_positions,
  std::vector<glm::vec3> t_normals,
  std::vector<glm::vec4> t_colors)
{
  std::vector<Vertex> vertices;

  // check for vector size equality
  if (not(t_positions.size() == t_normals.size() and t_positions.size() == t_colors.size())) {
    spdlog::debug(fmt::format("positions {}", t_positions.size()));
    spdlog::debug(fmt::format("normals {}", t_normals.size()));
    spdlog::debug(fmt::format("colors {}", t_colors.size()));
    throw InvalidArgumentLogged(fmt::format("vertex vector sizes do not match"));
  }

  for (std::size_t i{ 0 }; i < t_positions.size(); ++i) {
    vertices.emplace_back(
      t_positions.at(i), t_normals.at(i), glm::vec3(t_colors.at(i).r, t_colors.at(i).g, t_colors.at(i).b));
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
  glm::vec3 node_translation{ 0.0F, 0.0F, 0.0F };
  if (node.find("translation") != node.end()) {
    spdlog::debug("node has translation");
    std::array<float, 4> translation_values{ 0.0F };
    for (std::size_t i{ 0 }; i < node["translation"].size(); ++i) {
      translation_values.at(i) = (node["translation"][i]);
    }
    node_translation = glm::make_vec3(translation_values.data());
  }

  // rotation if exists
  glm::quat node_rotation{ glm::quat(1.0F, 0.0F, 0.0F, 0.0F) };
  if (node.find("rotation") != node.end()) {
    spdlog::debug("node has rotation");
    std::array<float, 4> rotation_values{
      node["rotation"][3], node["rotation"][0], node["rotation"][1], node["rotation"][2]
    };
    node_rotation = glm::make_quat(rotation_values.data());
  }

  // scale if exists
  glm::vec3 node_scale{ 1.0F, 1.0F, 1.0F };
  if (node.find("scale") != node.end()) {
    spdlog::debug("node has scaling");
    std::array<float, 3> scale_values{ 1.0F };
    for (std::size_t i{ 0 }; i < node["scale"].size(); ++i) { scale_values.at(i) = (node["scale"][i]); }
    node_scale = glm::make_vec3(scale_values.data());
  }

  // matrix if exists
  glm::mat4 node_matrix{ 1.0F };
  if (node.find("matrix") != node.end()) {
    spdlog::debug("node has matrix");
    std::array<float, 16> matrix_values;
    for (std::size_t i{ 0 }; i < node["matrix"].size(); ++i) { matrix_values.at(i) = (node["matrix"][i]); }
    node_matrix = glm::make_mat4(matrix_values.data());
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
  std::transform(
    color_vector.begin(), color_vector.end(), std::back_inserter(color_floats), map_to_float_range<uint16_t, float>());
  std::vector<glm::vec4> colors{ group_into_vec4<float>(color_floats) };

  // TODO: REMOVE THIS
  colors.emplace_back(1.0F, 1.0F, 1.0F, 1.0F);
  colors.emplace_back(1.0F, 1.0F, 1.0F, 1.0F);

  std::vector<uint8_t> position_bytes{ get_accessor_data(m_json["accessors"][position_access_index]) };
  std::vector<float> position_vector{ convert_bytes<float>(position_bytes) };
  std::vector<glm::vec3> positions{ group_into_vec3<float>(position_vector) };

  std::vector<uint8_t> normal_bytes{ get_accessor_data(m_json["accessors"][normal_access_index]) };
  std::vector<float> normal_vector{ convert_bytes<float>(normal_bytes) };
  std::vector<glm::vec3> normals{ group_into_vec3<float>(normal_vector) };

  std::vector<Vertex> vertices{ assemble_vertices(positions, normals, colors) };

  std::vector<uint8_t> indice_bytes{ get_accessor_data(m_json["accessors"][index_access_index]) };
  std::vector<uint16_t> indice_vector{ convert_bytes<uint16_t>(indice_bytes) };
  std::vector<GLuint> indices{ indice_vector.begin(), indice_vector.end() };

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
  ComponentType component_type = static_cast<ComponentType>(t_accessor["componentType"]);

  switch (component_type) {
  case ComponentType::INT8_T:
    spdlog::debug(fmt::format("type is {}, int8_t", static_cast<int>(component_type)));
    return h_get_accessor_data<int8_t>(t_accessor);
    break;

  case ComponentType::UINT8_T:
    spdlog::debug(fmt::format("type is {}, uint8_t", static_cast<int>(component_type)));
    return h_get_accessor_data<uint8_t>(t_accessor);
    break;

  case ComponentType::INT16_T:
    spdlog::debug(fmt::format("type is {}, int16_t", static_cast<int>(component_type)));
    return h_get_accessor_data<int16_t>(t_accessor);
    break;

  case ComponentType::UINT16_T:
    spdlog::debug(fmt::format("type is {}, uint16_t", static_cast<int>(component_type)));
    return h_get_accessor_data<uint16_t>(t_accessor);
    break;

  case ComponentType::UINT32_T:
    spdlog::debug(fmt::format("type is {}, uint32_t", static_cast<int>(component_type)));
    return h_get_accessor_data<uint32_t>(t_accessor);
    break;

  case ComponentType::FLOAT_T:
    spdlog::debug(fmt::format("type is {}, float_t", static_cast<int>(component_type)));
    return h_get_accessor_data<float_t>(t_accessor);
    break;

  default:
    throw RuntimeErrorLogged(fmt::format("unknown component type {}", static_cast<int>(component_type)));
  }
}