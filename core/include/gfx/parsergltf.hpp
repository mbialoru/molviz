#ifndef PARSERGLTF_HPP
#define PARSERGLTF_HPP

#pragma once

#include <nlohmann/json.hpp>

#include "mesh.hpp"

namespace Molviz::gfx {

class ParserGLTF
{
public:
  ParserGLTF(const char *tp_file);
  ParserGLTF(const std::string &tr_file);
  ParserGLTF(const std::filesystem::path &tr_file);
  ~ParserGLTF() = default;

private:
  std::vector<uint8_t> load_bin_file();

  template<typename T> std::vector<uint8_t> get_accessor_data(nlohmann::json t_accessor)
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

  template<typename T> std::vector<T> convert_accessor_data(const std::vector<uint8_t> &tr_data)
  {
    if (tr_data.size() % sizeof(T) != 0) {
      throw InvalidArgumentLogged(fmt::format("vector length not divisible by {}", sizeof(T)));
    }

    std::vector<T> converted_data;

    for (std::size_t i{ 0 }; i < tr_data.size() / sizeof(T); i += sizeof(T)) {
      std::array<uint8_t, sizeof(T)> bytes;
      for (std::size_t i{ 0 }; i < sizeof(T); ++i) { bytes[i] = m_data[i]; }
      T value;
      std::memmove(&value, bytes, sizeof(T));
      converted_data.push_back(value);
    }
    return converted_data;
  }

  std::filesystem::path m_path;

  std::vector<uint8_t> m_model_bin_data;

  nlohmann::json m_json;

  std::vector<uint8_t> m_data;
};

}// namespace Molviz::gfx

#endif// PARSERGLTF_HPP