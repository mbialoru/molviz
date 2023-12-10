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

template<typename T> std::vector<T> convert_bytes(const std::vector<uint8_t> &tr_data)
{
  if (tr_data.size() % sizeof(T) != 0) {
    throw InvalidArgumentLogged(fmt::format("vector length not divisible by {}", sizeof(T)));
  }

  std::vector<T> converted;

  for (std::size_t i{ 0 }; i < tr_data.size() / sizeof(T); i += sizeof(T)) {
    std::array<uint8_t, sizeof(T)> bytes;
    for (std::size_t i{ 0 }; i < sizeof(T); ++i) { bytes[i] = tr_data[i]; }
    T value;
    std::memmove(&value, bytes.data(), sizeof(T));
    converted.push_back(value);
  }
  return converted;
}

// unary functor
struct map_byte_to_float
{
  float operator()(const uint8_t t_input) const
  {
    return { static_cast<float>(t_input) / static_cast<float>(std::numeric_limits<uint8_t>::max()) };
  }
};

}// namespace Molviz

#endif// UTILITIES_HPP