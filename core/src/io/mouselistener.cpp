#include "io/mouselistener.hpp"
#include "exceptions.hpp"

using namespace mve::io;

void MouseListener::on_event(const Event &tr_event)
{
  // TODO: conditional cast to infer if its mouse movement or mouse click
  // const KeyEvent &key_event{ dynamic_cast<const KeyEvent &>(tr_event) };

  // std::stringstream sstream;

  // switch (key_event.get_type()) {
  // case EventType::KEY_PRESS:
  //   sstream << "Key Pressed ";
  //   break;
  // case EventType::KEY_RELEASE:
  //   sstream << "Key Released ";
  //   break;
  // case EventType::KEY_CONTINUOUS:
  //   sstream << "Key Continuous ";
  //   break;
  // case EventType::KEY_DOUBLE:
  //   sstream << "Key Double ";
  //   break;
  // default:
  //   throw InvalidEvent();
  //   break;
  // }

  // sstream << key_event.get_code();

  // spdlog::info(sstream.str());
}