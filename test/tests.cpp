#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include "libmorse/fft.hpp"
#include "libmorse/morse.hpp"

TEST_CASE("int_log2 works", "[int_log2]")
{
  REQUIRE(int_log2(8) == 3);
  REQUIRE(int_log2(64) == 6);
  REQUIRE(int_log2(1024) == 10);
  REQUIRE(int_log2(1048576) == 20);
}

TEST_CASE("reverse_number works", "[reverse_number]")
{
  REQUIRE(reverse_number(1, 1) == 0);
  REQUIRE(reverse_number(5, 5) == 2);
  REQUIRE(reverse_number(30, 30) == 7);
  REQUIRE(reverse_number(32, 32) == 0);
  REQUIRE(reverse_number(255, 65535) == 127);
}

TEST_CASE("reverse_order works", "[reverse_order]")
{
  auto array_values{ std::array<double, 10>{ 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 } };
  auto p_array{ std::make_shared<double[]>(10) };
  for (std::size_t i{ 0 }; i < 10; ++i) { p_array[i] = array_values[i]; }

  reverse_order(p_array, 4);

  REQUIRE(p_array[0] == 0.0);
  REQUIRE(p_array[1] == 1.0);
  REQUIRE(p_array[2] == 4.0);
  REQUIRE(p_array[3] == 5.0);
  REQUIRE(p_array[4] == 2.0);
  REQUIRE(p_array[5] == 3.0);
  REQUIRE(p_array[6] == 6.0);
  REQUIRE(p_array[7] == 7.0);
  REQUIRE(p_array[8] == 8.0);
  REQUIRE(p_array[9] == 9.0);
}
