#include "io/event.hpp"

using namespace mve::io;

Event::Event(EventType t_type) : m_type(t_type) {};

EventType Event::get_type() const { return m_type; }