#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#pragma once

#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <string>

#include "exceptions.hpp"

namespace Molviz {

std::string file_contents_to_string(const char *tp_file);
std::string file_contents_to_string(const std::string &tr_file);
std::string file_contents_to_string(const std::filesystem::path &tr_file);

template<typename T> std::vector<glm::vec<2, T, glm::defaultp>> group_data_vec2(const std::vector<T> &tr_data)
{
  if (tr_data.size() % 2 != 0) { throw InvalidArgumentLogged(fmt::format("vector length not divisible by {}", 2)); }

  std::vector<glm::vec<2, T, glm::defaultp>> grouped;

  for (std::size_t i{ 0 }; i < tr_data.size() / 2; i += 2) { grouped.emplace_back(tr_data[i + 0], tr_data[i + 1]); }

  return grouped;
};

template<typename T> std::vector<glm::vec<3, T, glm::defaultp>> group_data_vec3(const std::vector<T> &tr_data)
{
  if (tr_data.size() % 3 != 0) { throw InvalidArgumentLogged(fmt::format("vector length not divisible by {}", 3)); }

  std::vector<glm::vec<3, T, glm::defaultp>> grouped;

  for (std::size_t i{ 0 }; i < tr_data.size() / 3; i += 3) {
    grouped.emplace_back(tr_data[i + 0], tr_data[i + 1], tr_data[i + 2]);
  }
  return grouped;
};

template<typename T> std::vector<glm::vec<4, T, glm::defaultp>> group_data_vec4(const std::vector<T> &tr_data)
{
  if (tr_data.size() % 4 != 0) { throw InvalidArgumentLogged(fmt::format("vector length not divisible by {}", 4)); }

  std::vector<glm::vec<4, T, glm::defaultp>> grouped;

  for (std::size_t i{ 0 }; i < tr_data.size() / 4; i += 4) {
    grouped.emplace_back(tr_data[i + 0], tr_data[i + 1], tr_data[i + 2], tr_data[i + 3]);
  }
  return grouped;
};

template<typename T> std::size_t get_vertex_stride(std::string t_type)
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

}// namespace Molviz

#endif// UTILITIES_HPP