#ifndef EVENTDISPATCHER_HPP
#define EVENTDISPATCHER_HPP

#pragma once

#include "eventlistener.hpp"
#include <vector>

namespace mve::io {

class EventDispatcher
{
public:
  EventDispatcher() = default;
  ~EventDispatcher() = default;

  void add_listener(EventListener *tp_listener);
  void dispatch(const Event &tr_event);

private:
  std::vector<EventListener *> m_listeners;
};

}// namespace mve::io

#endif// EVENTDISPATCHER_HPP