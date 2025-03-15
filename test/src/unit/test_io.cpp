#include <catch2/catch_test_macros.hpp>
#include <random>

#include "io/event.hpp"
#include "io/eventdispatcher.hpp"
#include "io/keyboardlistener.hpp"
#include "io/keyevent.hpp"
#include "test_utilities.hpp"

CATCH_REGISTER_LISTENER(trace_log_level)

TEST_CASE("KeyboardListener key press", "[KeyboardListener]")
{
  using namespace mve::io;

  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<std::mt19937::result_type> distribution(0, 255);

  REQUIRE_NOTHROW([&]() {
    auto event_dispatcher{ EventDispatcher() };
    auto keyboard_listener{ KeyboardListener() };

    event_dispatcher.add_listener(&keyboard_listener);

    const KeyEvent a_press{ static_cast<int>(distribution(generator)), EventType::KEY_PRESS };

    event_dispatcher.dispatch(a_press);
  }());
}