#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#pragma once

#include <memory>
#include <type_traits>

namespace mve::cls {

template<typename T> class Singleton
{
public:
  static T &get_instance() noexcept(std::is_nothrow_constructible<T>::value) { return std::ref(*mp_instance); }
  static const T &get_instance_cref() noexcept(std::is_nothrow_constructible<T>::value)
  {
    return std::cref(*mp_instance);
  }

protected:
  Singleton() noexcept = default;
  Singleton(const Singleton &tr_other) = delete;
  virtual ~Singleton() = default;

  Singleton &operator=(const Singleton &tr_other) = delete;

private:
  static std::unique_ptr<T> mp_instance;
};

// NOTES: Oh yes, I _do_ know how fucking ugly this is.

}// namespace mve::cls

#endif// SINGLETON_HPP