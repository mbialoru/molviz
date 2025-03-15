#ifndef KEYBOARDLISTENER_HPP
#define KEYBOARDLISTENER_HPP

#pragma once

#include "eventlistener.hpp"

namespace mve::io {

class KeyboardListener : public EventListener
{
public:
  ~KeyboardListener() override = default;

  void on_event(const Event &tr_event) override;
};

}// namespace mve::io

#endif// KEYBOARDLISTENER_HPP