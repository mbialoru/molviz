#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#pragma once

#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <type_traits>

#include "exceptions.hpp"

namespace mve {

enum ComponentType { INT8_T = 5120, UINT8_T = 5121, INT16_T = 5122, UINT16_T = 5123, UINT32_T = 5125, FLOAT_T = 5126 };

std::string component_type_to_string(const ComponentType t_type);
std::string file_contents_to_string(const char *tp_file);
std::string file_contents_to_string(const std::string &tr_file);
std::string file_contents_to_string(const std::filesystem::path &tr_file);

bool file_exists(const char *tp_file);
bool file_exists(const std::string &tr_file);
bool file_exists(const std::filesystem::path &tr_file,
  std::filesystem::file_status t_s = std::filesystem::file_status{});

template<typename T> std::vector<T> convert_bytes(const std::vector<uint8_t> &tr_data)
{
  if (tr_data.size() % sizeof(T) != 0) {
    throw InvalidArgumentLogged(fmt::format("vector length not divisible by {}", sizeof(T)));
  }

  std::vector<T> converted;

  for (std::size_t i{ 0 }; i < tr_data.size(); i += sizeof(T)) {
    std::array<uint8_t, sizeof(T)> bytes;
    for (std::size_t j{ 0 }; j < sizeof(T); ++j) { bytes.at(j) = tr_data.at(i + j); }
    T value;
    std::memmove(&value, bytes.data(), sizeof(T));
    converted.push_back(value);
  }
  return converted;
}

// unary functor
template<typename T, typename P> struct map_to_float_range
{
  P operator()(const T t_input) const
  {
    if (not std::is_floating_point<P>::value) { throw InvalidArgumentLogged("expected floating point type"); }

    return { static_cast<P>(t_input) / static_cast<P>(std::numeric_limits<T>::max()) };
  }
};

}// namespace mve

#endif// UTILITIES_HPP