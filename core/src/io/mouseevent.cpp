#include "io/mouseevent.hpp"
#include "io/event.hpp"

using namespace mve::io;

MouseEvent::MouseEvent(int t_x, int t_y) : Event(EventType::MOUSE_MOVE), m_x(t_x), m_y(t_y) {};

int MouseEvent::get_x() const { return m_x; }

int MouseEvent::get_y() const { return m_y; }