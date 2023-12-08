#include "gfx/parsergltf.hpp"

using namespace Molviz::gfx;

ParserGLTF::ParserGLTF(const char *tp_file) : ParserGLTF(std::filesystem::path(std::string(tp_file))){};

ParserGLTF::ParserGLTF(const std::string &tr_file) : ParserGLTF(std::filesystem::path(tr_file)){};

ParserGLTF::ParserGLTF(const std::filesystem::path &tr_path) : m_path(tr_path) { throw NotImplementedException(); };

std::vector<uint8_t> ParserGLTF::load_bin_file()
{
  std::string uri = m_json["buffers"][0]["uri"];
  std::filesystem::path parent{ m_path.parent_path() };
  std::string bytes_text{ file_contents_to_string(parent / uri) };
  std::vector<uint8_t> data{ bytes_text.begin(), bytes_text.end() };

  return data;
};