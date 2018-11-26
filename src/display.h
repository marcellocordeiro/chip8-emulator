#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "common.hpp"

namespace chip8 {
class display {
public:
  display();
  void render(const std::array<bool, cte::gfx_size>&);

  actions poll_events();
  bool is_open();
  void close();

private:
  sf::RenderWindow _window;
};
}  // namespace chip8

/* #include <SDL.h>
#undef main

class display {
public:
  display();
  ~display();
  void poll_events();
  bool is_open();
  void close();

  void render(const std::array<bool, cte::gfx_size>&);

private:
  SDL_Window* _window;
  SDL_Renderer* _renderer;
}; */