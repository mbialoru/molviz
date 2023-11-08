#include "gfx/camera.hpp"

using namespace Molviz::gfx;

Camera::Camera(const int t_width, const int t_height, const glm::vec3 t_position)
  : position(t_position), orientation(glm::vec3(0.0F, 0.0F, -1.0F)), up(glm::vec3(0.0F, 1.0F, 0.0F)),
    camera_matrix(glm::mat4(1.0F)), first_click(true), width(t_width), height(t_height), speed(0.1F),
    sensitivity(100.0F){};

void Camera::update_matrix(const float t_fov_degrees, const float t_near_plane, const float t_far_plane)
{
  glm::mat4 view{ glm::mat4(1.0F) };
  glm::mat4 projection{ glm::mat4(1.0F) };

  view = glm::lookAt(position, position + orientation, up);
  projection =
    glm::perspective(glm::radians(t_fov_degrees), static_cast<float>(width / height), t_near_plane, t_far_plane);

  camera_matrix = projection * view;
}

void Camera::export_matrix(Shader &t_shader, const char *tp_uniform)
{
  glUniformMatrix4fv(glGetUniformLocation(t_shader.id, tp_uniform), 1, GL_FALSE, glm::value_ptr(camera_matrix));
}

void Camera::handle_inputs(const SDL_Event &tr_event)
{
  if (tr_event.type == SDL_KEYDOWN) {
    std::string message{ "key press: " };
    switch (tr_event.key.keysym.sym) {
    case SDLK_w:
      message += "W";
      position += speed * orientation;
      break;

    case SDLK_s:
      message += "S";
      position -= speed * orientation;
      break;

    case SDLK_a:
      message += "A";
      position -= speed * glm::normalize(glm::cross(orientation, up));
      break;

    case SDLK_d:
      message += "D";
      position += speed * glm::normalize(glm::cross(orientation, up));
      break;

    case SDLK_SPACE:
      message += "SPACE";
      position += speed * up;
      break;

    case SDLK_LCTRL:
      message += "LCTRL";
      position -= speed * up;
      break;

    case SDLK_LSHIFT:
      message += "LSHIFT";
      speed = 0.4F;
      break;
    }

    spdlog::debug(message);

  } else if (tr_event.type == SDL_KEYUP) {
    std::string message{ "key release: " };
    switch (tr_event.key.keysym.sym) {
    case SDLK_w:
      message += "W";
      break;

    case SDLK_s:
      message += "S";
      break;

    case SDLK_a:
      message += "A";
      break;

    case SDLK_d:
      message += "D";
      break;

    case SDLK_SPACE:
      message += "SPACE";
      break;

    case SDLK_LCTRL:
      message += "LCTRL";
      break;

    case SDLK_LSHIFT:
      message += "LSHIFT";
      speed = 0.1F;
      break;
    }

    spdlog::debug(message);
  }
}