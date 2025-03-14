#include "io/keyevent.hpp"

using namespace mve::io;

KeyEvent::KeyEvent(int t_key_code, EventType t_type) : Event(t_type), m_key_code(t_key_code) {};