#include "ctx/contextgfx.hpp"

using namespace mve::ctx;

template<> std::unique_ptr<ContextGFX> mve::cls::Singleton<ContextGFX>::mp_instance{ std::make_unique<ContextGFX>() };

ContextGFX::ContextGFX() : initialized(false) {};

void ContextGFX::initialize()
{
  if (not initialized) {
    if (is_initializable()) {
      initialized = true;
      p_camera = std::make_unique<gfx::Camera>(window_height, window_width, start_camera_position);
      p_shader = std::make_unique<gfx::Shader>(vertex_shader, fragment_shader);
    } else {
      throw RuntimeErrorLogged("ContextGFX initialization failed");
    }
  } else {
    throw RuntimeErrorLogged("ContextGFX double initialization attempt");
  }
}

bool ContextGFX::is_initializable()
{
  bool result{ true };// NOTE: start with true, and falsify if needed
  // TODO: check if internal parameters are valid for initialization

  if (not window_height > 0 or not window_width > 0) {
    spdlog::error("invalid window size");
    result = false;
  }

  if (not file_exists(vertex_shader)) {
    spdlog::error("vertex shader file does not exist");
    result = false;
  }

  if (not file_exists(fragment_shader)) {
    spdlog::error("fragment shader file does not exist");
    result = false;
  }

  if (not window_name.length() > 0) { spdlog::warn("window name is empty"); }

  spdlog::debug(
    "starting camera position is {} {} {}", start_camera_position.x, start_camera_position.y, start_camera_position.z);

  return result;
}