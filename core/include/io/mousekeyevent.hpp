#ifndef MOUSEKEYEVENT_HPP
#define MOUSEKEYEVENT_HPP

#pragma once

#include <cstdint>

#include "event.hpp"

namespace mve::io {

enum class MouseKey : std::uint8_t {
  MOUSE_LEFT,
  MOUSE_RIGHT,
  MOUSE_MIDDLE,
  MOUSE_WHEEL_UP,
  MOUSE_WHEEL_DOWN,
  MOUSE_WHEEL,
  MOUSE_THUMB_1,
  MOUSE_THUMB_2
};

class MouseKeyEvent : public Event
{
public:
  MouseKeyEvent(MouseKey t_key, EventType t_type);
  ~MouseKeyEvent() override = default;

  [[nodiscard]] MouseKey get_key() const;

private:
  MouseKey m_key;
};

}// namespace mve::io

#endif// MOUSEKEYEVENT_HPP