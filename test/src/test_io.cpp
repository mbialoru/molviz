#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <thread>

#include "test_utilities.hpp"
#include "test_utilities_gfx.hpp"

#include "io/inputhandler.hpp"

CATCH_REGISTER_LISTENER(trace_log_level)

TEST_CASE("InputHandler valid creation and cleanup", "[InputHandler]")
{
  using namespace Molviz::io;

  REQUIRE_NOTHROW([&]() { InputHandler input_handler; }());
}

TEST_CASE("InputHandler capturing input", "[InputHandler, Manual]")
{
  using namespace Molviz::io;
  using namespace std::chrono_literals;

  auto [p_window, context]{ create_real_opengl_context() };

  InputHandler input_handler;

  std::this_thread::sleep_for(15s);

  cleanup_opengl_context(p_window, context);
}