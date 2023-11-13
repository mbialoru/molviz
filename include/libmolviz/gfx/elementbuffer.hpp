#ifndef ELEMENTBUFFER_HPP
#define ELEMENTBUFFER_HPP

#pragma once

#include <GL/glew.h>
#include <vector>

namespace Molviz::gfx {

class ElementBuffer
{
public:
  explicit ElementBuffer(std::vector<GLuint> &tr_indices);
  ~ElementBuffer() = default;

  void bind();
  void unbind();
  void cleanup();

  GLuint id;
};

// convenient type alias
using EBO = ElementBuffer;

}// namespace Molviz::gfx

#endif// ELEMENTBUFFER_HPP