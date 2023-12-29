#include "gfx/camera.hpp"

using namespace mve::gfx;

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

void Camera::handle_inputs(const SDL_Event &tr_event, SDL_Window *tp_window)
{
  // TODO: input handling can have its own class, also event-based input is
  // worse than state-based input
  handle_inputs_keyboard(tr_event);
  handle_inputs_mouse(tr_event, tp_window);
}

void Camera::handle_inputs_keyboard(const SDL_Event &tr_event)
{
  std::string message;

  if (tr_event.type == SDL_KEYDOWN) {
    message += "(K) key down: ";
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
  } else if (tr_event.type == SDL_KEYUP) {
    message += "(K) key up: ";
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
  }
  if (message.length() > 0) { spdlog::debug(message); }
}

void Camera::handle_inputs_mouse(const SDL_Event &tr_event, SDL_Window *tp_window)
{
  std::string message;
  if (tr_event.type == SDL_MOUSEBUTTONDOWN) {
    message += "(M) key down: ";
    switch (tr_event.button.button) {
    case SDL_BUTTON_LEFT:
      message += "LMB";
      mouse_rotate(tp_window);
      break;

    case SDL_BUTTON_MIDDLE:
      message += "MMB";
      break;

    case SDL_BUTTON_RIGHT:
      message += "RMB";
      break;
    }
  } else if (tr_event.type == SDL_MOUSEBUTTONUP) {
    message += "(M) key up: ";
    switch (tr_event.button.button) {
    case SDL_BUTTON_LEFT:
      message += "LMB";
      mouse_reset();
      break;

    case SDL_BUTTON_MIDDLE:
      message += "MMB";
      break;

    case SDL_BUTTON_RIGHT:
      message += "RMB";
      break;
    }
  }

  if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_LMASK) {
    message += "(M) key hold: LMB";
    mouse_rotate(tp_window);
  }

  if (message.length() > 0) { spdlog::debug(message); }
}

void Camera::mouse_rotate(SDL_Window *tp_window)
{
  int window_width{ 0 };
  int window_height{ 0 };

  SDL_GetWindowSize(tp_window, &window_width, &window_height);
  SDL_ShowCursor(SDL_DISABLE);

  if (first_click) {
    SDL_WarpMouseInWindow(tp_window, (width / 2), (height / 2));
    first_click = false;
  }

  int mouse_x{ 0 };
  int mouse_y{ 0 };

  SDL_GetMouseState(&mouse_x, &mouse_y);

  float rotation_x{ sensitivity * static_cast<float>(mouse_y - (window_height / 2)) / window_height };
  float rotation_y{ sensitivity * static_cast<float>(mouse_x - (window_width / 2)) / window_width };

  glm::vec3 new_orientation{ glm::rotate(
    orientation, glm::radians(-rotation_x), glm::normalize(glm::cross(orientation, up))) };

  if (std::abs(glm::angle(new_orientation, up) - glm::radians(90.0F)) <= glm::radians(85.0F)) {
    orientation = new_orientation;
  }

  orientation = glm::rotate(orientation, glm::radians(-rotation_y), up);

  SDL_WarpMouseInWindow(tp_window, (window_width / 2), (window_height / 2));
}

void Camera::mouse_reset()
{
  SDL_ShowCursor(SDL_ENABLE);
  first_click = true;
}