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