#include "io/eventdispatcher.hpp"

using namespace mve::io;

void EventDispatcher::add_listener(EventListener *tp_listener) { m_listeners.push_back(tp_listener); }

void EventDispatcher::dispatch(const Event &tr_event)
{
  for (auto &listener : m_listeners) { listener->on_event(tr_event); }
}