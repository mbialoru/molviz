#ifndef KEYEVENT_HPP
#define KEYEVENT_HPP

#pragma once

#include "event.hpp"

namespace mve::io {

class KeyEvent : public Event
{
public:
  KeyEvent(int t_key_code, EventType t_type);
  ~KeyEvent() override = default;

  [[nodiscard]] int get_code() const;

private:
  int m_key_code;
};

}// namespace mve::io

#endif// KEYEVENT_HPP