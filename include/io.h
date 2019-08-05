#pragma once

#include <array>
#include <queue>

#include <SFML/Graphics.hpp>

#include "common.h"
#include "types/forward_decl.h"

namespace ct {
// array sizes
constexpr auto width  = 64;
constexpr auto height = 32;

// graphics
constexpr auto scale     = 20.0f;
constexpr auto res_scale = 20u;
constexpr auto fps_limit = 60u;

constexpr auto res_width  = width * res_scale;
constexpr auto res_height = height * res_scale;
}  // namespace ct

namespace chip8 {
class io {
public:
  io(chip8::emulator&);
  ~io();

  void close();
  void clear();

  uint32_t get_pixel(size_t, size_t) const;
  void     set_pixel(size_t, size_t, uint32_t);

  void draw();

  void run();

private:
  chip8::emulator& emulator;

  bool running = false;

  sf::RenderWindow window;

  std::array<sf::Color, ct::height * ct::width> frame_buffer{};
};
}  // namespace chip8
