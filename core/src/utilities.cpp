#include "utilities.hpp"

using namespace mve;

std::string mve::component_type_to_string(const ComponentType t_type)
{
  switch (t_type) {
  case ComponentType::INT8_T:
    return { "INT8_T" };
    break;

  case ComponentType::UINT8_T:
    return { "UINT8_T" };
    break;

  case ComponentType::INT16_T:
    return { "INT16_T" };
    break;

  case ComponentType::UINT16_T:
    return { "UINT16_T" };
    break;

  case ComponentType::UINT32_T:
    return { "UINT32_T" };
    break;

  case ComponentType::FLOAT_T:
    return { "FLOAT_T" };
    break;

  default:
    throw InvalidArgumentLogged(fmt::format("invalid component type {}", static_cast<int>(t_type)));
    break;
  }
}

std::string mve::file_contents_to_string(const char *tp_file)
{
  const std::filesystem::path file{ std::string(tp_file) };
  return file_contents_to_string(file);
}

std::string mve::file_contents_to_string(const std::string &tr_file)
{
  const std::filesystem::path file{ tr_file };
  return file_contents_to_string(file);
}

std::string mve::file_contents_to_string(const std::filesystem::path &tr_file)
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

bool mve::file_exists(const char *tp_file)
{
  const std::filesystem::path file{ std::string(tp_file) };
  return file_exists(file);
}

bool mve::file_exists(const std::string &tr_file)
{
  const std::filesystem::path file{ tr_file };
  return file_exists(file);
}

bool mve::file_exists(const std::filesystem::path &tr_file,
  std::filesystem::file_status t_status = std::filesystem::file_status{})
{
  if (std::filesystem::status_known(t_status) ? std::filesystem::exists(t_status) : std::filesystem::exists(tr_file)) {
    return true;
  }
  return false;
}