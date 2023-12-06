#include "utilities.hpp"

using namespace Molviz;

std::string Molviz::file_contents_to_string(const char *tp_file)
{
  const std::filesystem::path file{ std::string(tp_file) };
  return file_contents_to_string(file);
}

std::string Molviz::file_contents_to_string(const std::string &tr_file)
{
  const std::filesystem::path file{ tr_file };
  return file_contents_to_string(file);
}

std::string Molviz::file_contents_to_string(const std::filesystem::path &tr_file)
{
  std::string file_content;
  std::ifstream in_stream{ tr_file, std::ios::binary };

  if (in_stream && in_stream.good()) {
    in_stream.seekg(0, std::ios::end);
    file_content.resize(in_stream.tellg());
    in_stream.seekg(0, std::ios::beg);
    in_stream.read(file_content.data(), file_content.size());
    in_stream.close();
  } else {
    throw RuntimeErrorLogged(fmt::format("could not open {}", tr_file.string()));
  }
  return file_content;
}

std::vector<glm::vec2> Molviz::group_floats_vec2(const std::vector<float> &tr_floats)
{
  if (tr_floats.size() % 2 != 0) { throw RuntimeErrorLogged("vector size not divisible by 2"); }

  std::vector<glm::vec2> vectors;
  for (std::size_t i{ 0 }; i < tr_floats.size() / 2; i += 2) {
    vectors.emplace_back(tr_floats[i + 0], tr_floats[i + 1]);
  }
  return vectors;
}

std::vector<glm::vec3> Molviz::group_floats_vec3(const std::vector<float> &tr_floats)
{
  if (tr_floats.size() % 3 != 0) { throw RuntimeErrorLogged("vector size not divisible by 3"); }

  std::vector<glm::vec3> vectors;
  for (std::size_t i{ 0 }; i < tr_floats.size() / 3; i += 3) {
    vectors.emplace_back(tr_floats[i + 0], tr_floats[i + 1], tr_floats[i + 2]);
  }
  return vectors;
}

std::vector<glm::vec4> Molviz::group_floats_vec4(const std::vector<float> &tr_floats)
{
  if (tr_floats.size() % 4 != 0) { throw RuntimeErrorLogged("vector size not divisible by 4"); }

  std::vector<glm::vec4> vectors;
  for (std::size_t i{ 0 }; i < tr_floats.size() / 4; i += 4) {
    vectors.emplace_back(tr_floats[i + 0], tr_floats[i + 1], tr_floats[i + 2], tr_floats[i + 3]);
  }
  return vectors;
}