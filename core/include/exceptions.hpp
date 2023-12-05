#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#pragma once

#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>

namespace Molviz {

class NotImplementedException : public std::exception
{
public:
  NotImplementedException() : m_error_message("function not yet implemented") {}
  NotImplementedException(const std::string t_function_name) : NotImplementedException()
  {
    spdlog::error("{} {}", m_error_message, t_function_name);
  }
  NotImplementedException(const char *tp_function_name) : NotImplementedException(std::string(tp_function_name)) {}

  const char *what() const noexcept { return m_error_message.c_str(); }

private:
  std::string m_error_message;
};

class RuntimeErrorLogged : public std::exception
{
public:
  RuntimeErrorLogged(const std::string t_message) : m_error_message(t_message) { spdlog::error("{}", m_error_message); }
  RuntimeErrorLogged(const char *tp_message) : RuntimeErrorLogged(std::string(tp_message)) {}

  const char *what() const noexcept { return m_error_message.c_str(); }

private:
  std::string m_error_message;
};

class InvalidArgumentLogged : public std::exception
{
public:
  InvalidArgumentLogged(const std::string t_message) : m_error_message(t_message)
  {
    spdlog::error("{}", m_error_message);
  }
  InvalidArgumentLogged(const char *tp_message) : InvalidArgumentLogged(std::string(tp_message)) {}

  const char *what() const noexcept { return m_error_message.c_str(); }

private:
  std::string m_error_message;
};

}// namespace Molviz

#endif// EXCEPTIONS_HPP