#ifndef MOUSELISTENER_HPP
#define MOUSELISTENER_HPP

#pragma once

#include "eventlistener.hpp"
#include "mouseevent.hpp"
#include "mousekeyevent.hpp"

namespace mve::io {

class MouseListener : public EventListener
{
public:
  ~MouseListener() override = default;

  void on_event(const Event &tr_event) override;
  static void on_event(const MouseEvent &tr_event);
  static void on_event(const MouseKeyEvent &tr_event);
};

}// namespace mve::io

#endif// MOUSELISTENER_HPP