#include <catch2/catch_test_macros.hpp>
#include <random>

#include "io/event.hpp"
#include "io/eventdispatcher.hpp"
#include "io/keyboardlistener.hpp"
#include "io/keyevent.hpp"
#include "io/mouseevent.hpp"
#include "io/mousekeyevent.hpp"
#include "io/mouselistener.hpp"
#include "test_utilities.hpp"

CATCH_REGISTER_LISTENER(trace_log_level)

TEST_CASE("KeyboardListener single random", "[KeyboardListener]")
{
  using namespace mve::io;

  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<std::mt19937::result_type> distr(10, 100);
  std::uniform_int_distribution<std::mt19937::result_type> event_distr(0, 3);

  REQUIRE_NOTHROW([&]() {
    auto dispatcher{ EventDispatcher() };
    auto listener{ KeyboardListener() };

    dispatcher.add_listener(&listener);

    auto type = EventType(static_cast<int>(event_distr(generator)));

    const KeyEvent event{ static_cast<int>(distr(generator)), type };

    REQUIRE_NOTHROW([&]() { dispatcher.dispatch(event); }());
  }());
}

TEST_CASE("KeyboardListener multiple random", "[KeyboardListener]")
{
  using namespace mve::io;

  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<std::mt19937::result_type> key_distr(10, 100);
  std::uniform_int_distribution<std::mt19937::result_type> event_distr(0, 3);

  REQUIRE_NOTHROW([&]() {
    auto dispatcher{ EventDispatcher() };
    auto listener{ KeyboardListener() };

    dispatcher.add_listener(&listener);

    std::vector<KeyEvent> events{};

    for (std::size_t i{ 0 }; i < static_cast<int>(key_distr(generator)); ++i) {
      auto type = EventType(static_cast<int>(event_distr(generator)));
      events.emplace_back(static_cast<int>(key_distr(generator)), type);
    }

    for (const auto &r_event : events) {
      REQUIRE_NOTHROW([&]() { dispatcher.dispatch(r_event); }());
    }
  }());
}

TEST_CASE("MouseListener move single random", "[MouseListener]")
{
  using namespace mve::io;

  std::random_device device;
  std::mt19937 generator(device());

  std::uniform_int_distribution<std::mt19937::result_type> event_distr(0, 4);
  std::uniform_int_distribution<std::mt19937::result_type> key_distr(0, 7);
  std::uniform_int_distribution<std::mt19937::result_type> pos_distr(0, 127);

  REQUIRE_NOTHROW([&]() {
    auto dispatcher{ EventDispatcher() };
    auto listener{ MouseListener() };

    dispatcher.add_listener(&listener);

    const MouseEvent event{ static_cast<int>(event_distr(generator)), static_cast<int>(event_distr(generator)) };

    REQUIRE_NOTHROW([&]() { dispatcher.dispatch(event); }());
  }());
}

TEST_CASE("MouseListener key single random", "[MouseListener]")
{
  using namespace mve::io;

  std::random_device device;
  std::mt19937 generator(device());

  std::uniform_int_distribution<std::mt19937::result_type> event_distr(0, 4);
  std::uniform_int_distribution<std::mt19937::result_type> key_distr(0, 7);

  REQUIRE_NOTHROW([&]() {
    auto dispatcher{ EventDispatcher() };
    auto listener{ MouseListener() };

    dispatcher.add_listener(&listener);

    auto type = EventType(static_cast<int>(event_distr(generator)));
    auto key = MouseKey(static_cast<int>(key_distr(generator)));

    const MouseKeyEvent event{ key, type };

    REQUIRE_NOTHROW([&]() { dispatcher.dispatch(event); }());
  }());
}

TEST_CASE("MouseListener move key random", "[MouseListener]")
{
  using namespace mve::io;

  std::random_device device;
  std::mt19937 generator(device());

  std::uniform_int_distribution<std::mt19937::result_type> event_distr(0, 4);
  std::uniform_int_distribution<std::mt19937::result_type> key_distr(0, 7);
  std::uniform_int_distribution<std::mt19937::result_type> pos_distr(0, 127);

  REQUIRE_NOTHROW([&]() {
    auto dispatcher{ EventDispatcher() };
    auto listener{ MouseListener() };

    dispatcher.add_listener(&listener);

    auto type = EventType(static_cast<int>(event_distr(generator)));
    auto key = MouseKey(static_cast<int>(key_distr(generator)));

    const MouseEvent move_event{ static_cast<int>(event_distr(generator)), static_cast<int>(event_distr(generator)) };
    const MouseKeyEvent key_event{ key, type };

    REQUIRE_NOTHROW([&]() { dispatcher.dispatch(move_event); }());
    REQUIRE_NOTHROW([&]() { dispatcher.dispatch(key_event); }());
  }());
}