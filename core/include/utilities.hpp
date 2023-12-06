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

std::vector<glm::vec2> group_floats_vec2(const std::vector<float> &tr_floats);
std::vector<glm::vec3> group_floats_vec3(const std::vector<float> &tr_floats);
std::vector<glm::vec4> group_floats_vec4(const std::vector<float> &tr_floats);

}// namespace Molviz

#endif// UTILITIES_HPP