#include "utilities.hpp"

using namespace Molviz;

std::string Molviz::file_contents_to_string(const char *tp_file)
{
  std::filesystem::path file{ std::string(tp_file) };
  return file_contents_to_string(file);
}

std::string Molviz::file_contents_to_string(std::string t_file)
{
  std::filesystem::path file{ t_file };
  return file_contents_to_string(file);
}

std::string Molviz::file_contents_to_string(std::filesystem::path t_file)
{
  std::string file_content;
  std::ifstream in_stream{ t_file, std::ios::binary };

  if (in_stream && in_stream.good()) {
    in_stream.seekg(0, std::ios::end);
    file_content.resize(in_stream.tellg());
    in_stream.seekg(0, std::ios::beg);
    in_stream.read(file_content.data(), file_content.size());
    in_stream.close();
  } else {
    spdlog::error("could not open {}", t_file.string());
  }

  return file_content;
}