#include "io/keyboardlistener.hpp"
#include "exceptions.hpp"
#include "io/keyevent.hpp"
#include <spdlog/spdlog.h>
#include <sstream>


using namespace mve::io;

void KeyboardListener::on_event(const Event &tr_event)
{
  const KeyEvent &key_event{ dynamic_cast<const KeyEvent &>(tr_event) };

  std::stringstream sstream;

  switch (key_event.get_type()) {
  case EventType::KEY_PRESS:
    sstream << "Key Pressed ";
    break;
  case EventType::KEY_RELEASE:
    sstream << "Key Released ";
    break;
  case EventType::KEY_CONTINUOUS:
    sstream << "Key Continuous ";
    break;
  case EventType::KEY_DOUBLE:
    sstream << "Key Double ";
    break;
  default:
    throw InvalidEvent();
    break;
  }

  sstream << key_event.get_code();

  spdlog::info(sstream.str());
}