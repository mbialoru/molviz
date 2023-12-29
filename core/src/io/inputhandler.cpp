#include "io/inputhandler.hpp"

using namespace mve::io;

InputHandler::InputHandler() : m_worker_enabled(true)
{
  spdlog::trace("launching InputHandler thread");
  m_worker_thread = std::thread(&InputHandler::worker_loop, this);
  m_mouse_state = SDL_GetMouseState(nullptr, nullptr);
  mp_keyboard_state = SDL_GetKeyboardState(nullptr);
};

InputHandler::~InputHandler()
{
  m_worker_enabled = false;
  if (m_worker_thread.joinable()) { m_worker_thread.join(); }
}

void InputHandler::worker_loop()
{
  while (m_worker_enabled) {
    SDL_PollEvent(&m_event);
    m_mouse_state = SDL_GetMouseState(nullptr, nullptr);
    mp_keyboard_state = SDL_GetKeyboardState(nullptr);
    handle_input_actions();
    std::this_thread::sleep_for(m_ups_delay);
  }
}

void InputHandler::handle_input_actions()
{
  // keyboard input
  if (mp_keyboard_state[SDL_SCANCODE_SPACE]) { spdlog::trace("SPACE is pressed"); }
  if (mp_keyboard_state[SDL_SCANCODE_LCTRL]) { spdlog::trace("LCTRL is pressed"); }
  if (mp_keyboard_state[SDL_SCANCODE_W]) { spdlog::trace("W is pressed"); }
  if (mp_keyboard_state[SDL_SCANCODE_A]) { spdlog::trace("A is pressed"); }
  if (mp_keyboard_state[SDL_SCANCODE_S]) { spdlog::trace("S is pressed"); }
  if (mp_keyboard_state[SDL_SCANCODE_D]) { spdlog::trace("D is pressed"); }

  // mouse input
  if ((m_mouse_state & SDL_BUTTON_LMASK)) { spdlog::trace("LMB is pressed"); }
  if ((m_mouse_state & SDL_BUTTON_MMASK)) { spdlog::trace("MMB is pressed"); }
  if ((m_mouse_state & SDL_BUTTON_RMASK)) { spdlog::trace("RMB is presses"); }
}