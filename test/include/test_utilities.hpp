#ifndef TEST_UTILITIES_HPP
#define TEST_UTILITIES_HPP

#pragma once

#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <spdlog/spdlog.h>

// event listener to set loglevel to debug
class debug_log_level : public Catch::EventListenerBase
{
public:
  using Catch::EventListenerBase::EventListenerBase;
  void testRunStarting(Catch::TestRunInfo const &_) override { spdlog::set_level(spdlog::level::debug); }
};

// event listener  to disable log output
class disable_loging : public Catch::EventListenerBase
{
public:
  using Catch::EventListenerBase::EventListenerBase;
  void testRunStarting(Catch::TestRunInfo const &_) override { spdlog::set_level(spdlog::level::off); }
};

#endif// TEST_UTILITIES_HPP