#ifndef CONTEXTGFX_HPP
#define CONTEXTGFX_HPP

#pragma once

#include "cls/singleton.hpp"
#include "gfx/camera.hpp"
#include "gfx/shader.hpp"

namespace Molviz::ctx {

class ContextGFX final : public cls::Singleton<ContextGFX>
{
  friend class cls::Singleton<ContextGFX>;
  // NOTE: default_delete is important for unique_ptr in Singleton to work
  friend class std::default_delete<ContextGFX>;

public:
  std::unique_ptr<gfx::Camera> p_camera;
  std::unique_ptr<gfx::Shader> p_shader;

private:
  ContextGFX();
  ~ContextGFX() = default;
};

}// namespace Molviz::ctx

#endif// CONTEXTGFX_HPP