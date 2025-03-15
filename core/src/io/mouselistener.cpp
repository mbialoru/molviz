#include "io/mouselistener.hpp"
#include "exceptions.hpp"
#include <spdlog/spdlog.h>
#include <typeinfo>

using namespace mve::io;

void MouseListener::on_event(const Event &tr_event)
{
  try {
    const MouseEvent &r_mouse_event{ dynamic_cast<const MouseEvent &>(tr_event) };
    on_event(r_mouse_event);
  } catch (const std::bad_cast &e) {
    spdlog::debug("not a mouse event");
  }

  try {
    const MouseKeyEvent &r_mouse_key_event{ dynamic_cast<const MouseKeyEvent &>(tr_event) };
    on_event(r_mouse_key_event);
  } catch (const std::bad_cast &e) {
    spdlog::debug("not a mouse key event");
  }

  throw InvalidEvent();
}

void MouseListener::on_event(const MouseEvent &tr_event)
{
  std::stringstream sstream;

  switch (tr_event.get_type()) {
  case EventType::MOUSE_MOVE:
    sstream << "Mouse Moved ";
    break;
  default:
    throw InvalidEvent();
    break;
  }

  sstream << tr_event.get_x() << tr_event.get_y();

  spdlog::info(sstream.str());
}

void MouseListener::on_event(const MouseKeyEvent &tr_event)
{
  std::stringstream sstream;

  switch (tr_event.get_type()) {
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

  sstream << static_cast<int>(tr_event.get_key());

  spdlog::info(sstream.str());
}