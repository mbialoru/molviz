#include "io/mousekeyevent.hpp"

using namespace mve::io;

MouseKeyEvent::MouseKeyEvent(MouseKey t_key, EventType t_type) : Event(t_type), m_key(t_key) {};

MouseKey MouseKeyEvent::get_key() const { return m_key; }