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

bool Molviz::file_exists(const char *tp_file)
{
  const std::filesystem::path file{ std::string(tp_file) };
  return file_exists(file);
}

bool Molviz::file_exists(const std::string &tr_file)
{
  const std::filesystem::path file{ tr_file };
  return file_exists(file);
}

bool Molviz::file_exists(const std::filesystem::path &tr_file,
  std::filesystem::file_status t_status = std::filesystem::file_status{})
{
  if (std::filesystem::status_known(t_status) ? std::filesystem::exists(t_status) : std::filesystem::exists(tr_file)) {
    return true;
  }
  return false;
}