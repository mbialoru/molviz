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
  friend std::unique_ptr<ContextGFX> std::make_unique<ContextGFX>();

public:
  void initialize();
  bool is_initializable();

  std::size_t window_height;
  std::size_t window_width;

  std::string window_name;

  std::filesystem::path vertex_shader;
  std::filesystem::path fragment_shader;

  glm::vec3 start_camera_position;

  std::unique_ptr<gfx::Camera> p_camera;
  std::unique_ptr<gfx::Shader> p_shader;

private:
  ContextGFX();
  ~ContextGFX() = default;

  bool initialized;
};

}// namespace Molviz::ctx

#endif// CONTEXTGFX_HPP