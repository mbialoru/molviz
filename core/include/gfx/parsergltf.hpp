#ifndef PARSERGLTF_HPP
#define PARSERGLTF_HPP

#pragma once

#include <nlohmann/json.hpp>

#include "model.hpp"

namespace Molviz::gfx {

class ParserGLTF
{
public:
  ParserGLTF();
  ParserGLTF(const char *tp_file);
  ParserGLTF(const std::string &tr_file);
  ParserGLTF(const std::filesystem::path &tr_file);
  ~ParserGLTF() = default;

  void parse(const char *tp_file);
  void parse(const std::string &tr_file);
  void parse(const std::filesystem::path &tr_file);

  Model get_model();

private:
  std::vector<uint8_t> load_bin_file();

  std::vector<Vertex> assemble_vertices(std::vector<glm::vec3> t_positions,
    std::vector<glm::vec3> t_normals,
    std::vector<glm::vec4> t_colors);

  void process_node(const std::size_t t_next_node, const glm::mat4 t_matrix = glm::mat4(1.0F));
  void load_mesh(const std::size_t t_mesh_index);
  void cleanup();

  template<typename T> std::vector<glm::vec<2, T, glm::defaultp>> group_into_vec2(const std::vector<T> &tr_data)
  {
    if (tr_data.size() % 2 != 0) { throw InvalidArgumentLogged(fmt::format("vector length not divisible by {}", 2)); }

    std::vector<glm::vec<2, T, glm::defaultp>> grouped;

    for (std::size_t i{ 0 }; i < tr_data.size() / 2; i += 2) { grouped.emplace_back(tr_data[i + 0], tr_data[i + 1]); }

    return grouped;
  };

  template<typename T> std::vector<glm::vec<3, T, glm::defaultp>> group_into_vec3(const std::vector<T> &tr_data)
  {
    if (tr_data.size() % 3 != 0) { throw InvalidArgumentLogged(fmt::format("vector length not divisible by {}", 3)); }

    std::vector<glm::vec<3, T, glm::defaultp>> grouped;

    for (std::size_t i{ 0 }; i < tr_data.size() / 3; i += 3) {
      grouped.emplace_back(tr_data[i + 0], tr_data[i + 1], tr_data[i + 2]);
    }
    return grouped;
  };

  template<typename T> std::vector<glm::vec<4, T, glm::defaultp>> group_into_vec4(const std::vector<T> &tr_data)
  {
    if (tr_data.size() % 4 != 0) { throw InvalidArgumentLogged(fmt::format("vector length not divisible by {}", 4)); }

    std::vector<glm::vec<4, T, glm::defaultp>> grouped;

    for (std::size_t i{ 0 }; i < tr_data.size() / 4; i += 4) {
      grouped.emplace_back(tr_data[i + 0], tr_data[i + 1], tr_data[i + 2], tr_data[i + 3]);
    }
    return grouped;
  };

  template<typename T> std::size_t get_vertex_stride(const std::string t_type)
  {
    if (t_type == "SCALAR")
      return 1;
    else if (t_type == "VEC2")
      return sizeof(glm::vec<2, T, glm::qualifier::defaultp>) / sizeof(T);
    else if (t_type == "VEC3")
      return sizeof(glm::vec<3, T, glm::qualifier::defaultp>) / sizeof(T);
    else if (t_type == "VEC4")
      return sizeof(glm::vec<4, T, glm::qualifier::defaultp>) / sizeof(T);
    else {
      throw InvalidArgumentLogged(fmt::format("type is invalid {} given", t_type));
    }
  }

  template<typename T> std::vector<uint8_t> h_get_accessor_data(nlohmann::json t_accessor)
  {
    //!! WARNING: do not use curly brace initialization with json lib types, internal types mismatch shenanigans
    std::size_t buffer_view_index = t_accessor.value("bufferView", 1);
    std::size_t count = t_accessor["count"];
    std::size_t access_byte_offset = t_accessor.value("byteOffset", 0);
    std::string type = t_accessor["type"];

    nlohmann::json buffer_view = m_json["bufferViews"][buffer_view_index];

    std::size_t byte_offset{ buffer_view["byteOffset"] };

    std::vector<uint8_t> data;

    std::size_t data_begin = byte_offset + access_byte_offset;
    std::size_t data_size = count * sizeof(T) * get_vertex_stride<T>(type);

    for (std::size_t i{ data_begin }; i < data_begin + data_size; i += sizeof(T)) {
      for (std::size_t i{ 0 }; i < sizeof(T); ++i) { data.push_back(m_data[i]); }
    }
  }

  std::vector<uint8_t> get_accessor_data(nlohmann::json t_accessor);

  std::vector<uint8_t> m_data;

  std::filesystem::path m_path;

  nlohmann::json m_json;

  std::vector<Mesh> m_meshes;
  std::vector<glm::vec3> m_translations;
  std::vector<glm::vec3> m_scales;
  std::vector<glm::quat> m_rotations;
  std::vector<glm::mat4> m_matrices;
};

}// namespace Molviz::gfx

#endif// PARSERGLTF_HPP