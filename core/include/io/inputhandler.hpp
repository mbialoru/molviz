#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#pragma once

#include <SDL.h>
#include <condition_variable>
#include <mutex>
#include <thread>

// TODO: run on separate thread
// TODO: keep updating mouse and keyboard with time resolution (UPS, updates per
// second)
// TODO: allow to hook action when a button is pressed, released or held
// NOTE: do not use events, they rely on keyboard typing API like A...AAAAAA

namespace Molviz::io {

class InputHandler
{
public:
  explicit InputHandler();
  ~InputHandler() = default;

private:
  void get_keyboard_state();
  void get_mouse_state();
  void worker_loop();

  std::thread m_worker_thread;
  std::mutex m_mutex;
  std::unique_lock<std::mutex> m_lock;
  std::condition_variable m_condition_variable;
};

}// namespace Molviz::io

#endif// INPUTHANDLER_HPP