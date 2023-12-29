#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#pragma once

#include <SDL.h>
#include <atomic>
#include <spdlog/spdlog.h>
#include <thread>

// TODO: run on separate thread
// TODO: keep updating mouse and keyboard with time resolution (UPS, updates per
// second)
// TODO: allow to hook action when a button is pressed, released or held
// that one we'll do with action functions when we implement contexts
// NOTE: do not use events, they rely on keyboard typing API like A...AAAAAA

constexpr intmax_t UPS_VALUE{ 60 };

namespace mve::io {

class InputHandler
{
public:
  explicit InputHandler();
  ~InputHandler();

private:
  void worker_loop();
  void handle_input_actions();

  const Uint8 *mp_keyboard_state;
  uint32_t m_mouse_state;

  SDL_Event m_event;

  std::chrono::duration<double, std::ratio<1, UPS_VALUE>> m_ups_delay{ 1 };
  std::atomic_bool m_worker_enabled;
  std::thread m_worker_thread;
};

}// namespace mve::io

#endif// INPUTHANDLER_HPP