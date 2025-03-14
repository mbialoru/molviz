#ifndef EVENT_HPP
#define EVENT_HPP

#pragma once

#include <cstdint>

namespace mve::io {

enum class EventType : std::uint8_t {
  KEY_PRESSED,
  KEY_RELEASED,
  MOUSE_MOVED,
  MOUSE_BUTTON_PRESSED,
  MOUSE_BUTTON_RELEASED
};

class Event
{
public:
  explicit Event(EventType t_type);
  virtual ~Event() = default;

  [[nodiscard]] EventType get_type() const;

protected:
  EventType m_type;
};

}// namespace mve::io

#endif// EVENT_HPP