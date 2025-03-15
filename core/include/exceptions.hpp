#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#pragma once

#include <spdlog/spdlog.h>
#include <string>
#include <utility>

namespace mve {

class NotImplementedException : public std::exception
{
public:
  NotImplementedException() : m_error_message("function not yet implemented") {}
  explicit NotImplementedException(const std::string &tr_function_name) : NotImplementedException()
  {
    spdlog::error("{} {}", m_error_message, tr_function_name);
  }
  explicit NotImplementedException(const char *tp_function_name)
    : NotImplementedException(std::string(tp_function_name))
  {}

  [[nodiscard]] const char *what() const noexcept override { return m_error_message.c_str(); }

private:
  std::string m_error_message;
};

class InvalidEvent : public std::exception
{
public:
  InvalidEvent() : m_error_message("invalid input event type") {}
  explicit InvalidEvent(std::string t_event_type) : InvalidEvent()
  {
    spdlog::error("{} {}", m_error_message, t_event_type);
  }
  explicit InvalidEvent(const char *tp_event_type) : InvalidEvent(std::string(tp_event_type)) {}

  [[nodiscard]] const char *what() const noexcept override { return m_error_message.c_str(); }

private:
  std::string m_error_message;
};

class RuntimeErrorLogged : public std::exception
{
public:
  explicit RuntimeErrorLogged(std::string t_message) : m_error_message(std::move(t_message))
  {
    spdlog::error("{}", m_error_message);
  }
  explicit RuntimeErrorLogged(const char *tp_message) : RuntimeErrorLogged(std::string(tp_message)) {}

  [[nodiscard]] const char *what() const noexcept override { return m_error_message.c_str(); }

private:
  std::string m_error_message;
};

class InvalidArgumentLogged : public std::exception
{
public:
  explicit InvalidArgumentLogged(std::string t_message) : m_error_message(std::move(t_message))
  {
    spdlog::error("{}", m_error_message);
  }
  explicit InvalidArgumentLogged(const char *tp_message) : InvalidArgumentLogged(std::string(tp_message)) {}

  [[nodiscard]] const char *what() const noexcept override { return m_error_message.c_str(); }

private:
  std::string m_error_message;
};

}// namespace mve

#endif// EXCEPTIONS_HPP