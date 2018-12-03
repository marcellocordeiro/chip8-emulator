#pragma once

#include <SDL2/SDL.h>
#include <array>
#include <memory>

namespace ct {
// array sizes
constexpr auto gfx_width = 64;
constexpr auto gfx_height = 32;

// graphics
constexpr auto scale = 20.0f;
constexpr auto res_scale = 20u;
constexpr auto fps_limit = 60u;

constexpr auto res_width = gfx_width * res_scale;
constexpr auto res_height = gfx_height * res_scale;
}  // namespace ct

enum class ac { none, pause, reset };

namespace SDL2 {
struct Deleter {
  void operator()(SDL_Renderer* ptr) {
    if (ptr) {
      SDL_DestroyRenderer(ptr);
    }
  }

  void operator()(SDL_Window* ptr) {
    if (ptr) {
      SDL_DestroyWindow(ptr);
    }
  }
};

using Renderer = std::unique_ptr<SDL_Renderer, Deleter>;
using Window = std::unique_ptr<SDL_Window, Deleter>;
}  // namespace SDL2

namespace chip8 {
class display {
public:
  display();
  bool is_open();
  void close();
  void clear();

  ac poll_events();
  void render();

  const SDL_Color& get_pixel(std::size_t, std::size_t) const;
  void set_pixel(std::size_t, std::size_t, const SDL_Color&);

private:
  SDL2::Window _window;
  SDL2::Renderer _renderer;
  std::array<std::array<SDL_Color, ct::gfx_height>, ct::gfx_width> _gfx = {{}};
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

  void render(const std::array<bool, ct::gfx_size>&);

private:
  SDL_Window* _window;
  SDL_Renderer* _renderer;
}; */