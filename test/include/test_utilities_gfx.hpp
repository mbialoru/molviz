#ifndef TEST_GFX_UTILITIES_HPP
#define TEST_GFX_UTILITIES_HPP

#pragma once

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <catch2/catch_test_macros.hpp>
#include <spdlog/spdlog.h>

std::pair<SDL_Window *, SDL_GLContext> create_dummy_opengl_context();
std::pair<SDL_Window *, SDL_GLContext> create_real_opengl_context();

void cleanup_opengl_context(SDL_Window *tp_window, SDL_GLContext t_context);

#endif// TEST_GFX_UTILITIES_HPP