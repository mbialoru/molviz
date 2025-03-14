#ifndef MOUSEEVENT_HPP
#define MOUSEEVENT_HPP

#pragma once

#include "event.hpp"

namespace mve::io {

class MouseEvent : public Event
{
public:
  MouseEvent(int t_x, int t_y);
  ~MouseEvent() override = default;

private:
  int m_x;
  int m_y;
};

}// namespace mve::io

#endif// MOUSEEVENT_HPP