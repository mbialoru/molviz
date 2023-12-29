#ifndef ELEMENTBUFFER_HPP
#define ELEMENTBUFFER_HPP

#pragma once

#include <GL/glew.h>
#include <vector>

namespace mve::gfx {

class ElementBuffer
{
public:
  explicit ElementBuffer(std::vector<GLuint> &tr_indices);
  ~ElementBuffer();

  void bind();
  void unbind();
  void cleanup();

  GLuint id;
};

// convenient type alias
using EBO = ElementBuffer;

}// namespace mve::gfx

#endif// ELEMENTBUFFER_HPP