#ifndef MOUSELISTENER_HPP
#define MOUSELISTENER_HPP

#pragma once

#include "eventlistener.hpp"

namespace mve::io {

class MouseListener : public EventListener
{
public:
  ~MouseListener() override = default;

  void on_event(const Event &tr_event) override;
};

}// namespace mve::io

#endif// MOUSELISTENER_HPP