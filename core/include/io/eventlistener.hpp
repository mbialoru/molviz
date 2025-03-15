#ifndef EVENTLISTENER_HPP
#define EVENTLISTENER_HPP

#pragma once

#include "event.hpp"

namespace mve::io {

class EventListener
{
public:
  virtual ~EventListener() = default;

  virtual void on_event(const Event &tr_event) = 0;
};

}// namespace mve::io

#endif// EVENTLISTENER_HPP