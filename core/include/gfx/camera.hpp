#ifndef CAMERA_HPP
#define CAMERA_HPP

#pragma once

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "exceptions.hpp"
#include "shader.hpp"

namespace mve::gfx {

class Camera
{
public:
  Camera(const int t_width, const int t_height, const glm::vec3 t_position);
  ~Camera() = default;

  void update_matrix(const float t_fov_degrees, const float t_near_plane, const float t_far_plane);
  void export_matrix(mve::gfx::Shader &t_shader, const char *tp_uniform);
  void handle_inputs(const SDL_Event &tr_event, SDL_Window *tp_window);

  glm::vec3 position;
  glm::vec3 orientation;
  glm::vec3 up;
  glm::mat4 camera_matrix;

  bool first_click;

  int width;
  int height;

  float speed;
  float sensitivity;

private:
  void handle_inputs_keyboard(const SDL_Event &tr_event);
  void handle_inputs_mouse(const SDL_Event &tr_event, SDL_Window *tp_window);
  void mouse_rotate(SDL_Window *tp_window);
  void mouse_reset();
};

}// namespace mve::gfx

#endif// CAMERA_HPP