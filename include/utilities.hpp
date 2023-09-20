#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#pragma once

#include <filesystem>
#include <fstream>
#include <spdlog/spdlog.h>
#include <string>

namespace Molviz {

std::string file_contents_to_string(const char *tp_file);
std::string file_contents_to_string(std::string t_file);
std::string file_contents_to_string(std::filesystem::path t_file);

}// namespace Molviz

#endif// UTILITIES_HPP