#ifndef TEST_GFX_UTILITIES_HPP
#define TEST_GFX_UTILITIES_HPP

#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <catch2/catch_test_macros.hpp>
#include <spdlog/spdlog.h>

// create dummy zero-size window for valid opengl context
std::pair<SDL_Window *, SDL_GLContext> create_dummy_opengl_context()
{
  // init SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    spdlog::error("SDL initialization error: {}", SDL_GetError());
    FAIL();
  }

  // create dummy hidden SDL window for a dummy OpenGL context
  SDL_WindowFlags window_flags{ static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN) };
  SDL_Window *p_window{ SDL_CreateWindow(nullptr, 0, 0, 0, 0, window_flags) };
  SDL_GLContext context{ SDL_GL_CreateContext(p_window) };

  // init GLEW after OpenGL context - Missing GL version error otherwise
  GLenum status{ glewInit() };
  if (status != GLEW_OK) {
    std::string error_message{ reinterpret_cast<const char *>(glewGetErrorString(status)) };
    spdlog::error("GLEW init failed! error: {}", error_message);
    FAIL();
  }

  return { p_window, context };
}

// delete context and destroy window
void cleanup_opengl_context(SDL_Window *tp_window, SDL_GLContext t_context)
{
  SDL_GL_DeleteContext(t_context);
  SDL_DestroyWindow(tp_window);
  SDL_Quit();
}

#endif// TEST_GFX_UTILITIES_HPP