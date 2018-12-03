#include "display.h"

namespace chip8 {
display::display() {
  _window.reset(SDL_CreateWindow("chip8-emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ct::res_width, ct::res_height, SDL_WINDOW_SHOWN));
  _renderer.reset(SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED));
}

bool display::is_open() {
  return (_renderer != nullptr);
}

void display::close() {
  _window = nullptr;
  _renderer = nullptr;
}

ac display::poll_events() {
  auto action = ac::none;
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      close();
    }

    /* if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
      action = ac::pause;
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1) {
      action = ac::reset;
    } */
  }

  return action;
}

void display::clear() {
  /* std::for_each(_gfx.begin(), _gfx.end(),
    [](auto&& arr) { arr.fill(SDL_Color::Black); }
  ); */ // for documentation purposes

  _gfx.fill({});

  render();
}

const SDL_Color& display::get_pixel(std::size_t x, std::size_t y) const {
  return _gfx[x][y];
}

void display::set_pixel(std::size_t x, std::size_t y, const SDL_Color& color) {
  _gfx[x][y] = color;
}

void display::render() {
  SDL_Rect rect;
  rect.h = ct::res_scale;
  rect.w = ct::res_scale;

  for (std::size_t y = 0; y < ct::gfx_height; ++y) {
    for (std::size_t x = 0; x < ct::gfx_width; ++x) {
      rect.x = x * ct::res_scale;
      rect.y = y * ct::res_scale;
      auto c = _gfx[x][y];

      SDL_SetRenderDrawColor(_renderer.get(), c.r, c.g, c.b, c.a);
      SDL_RenderFillRect(_renderer.get(), &rect);
    }
  }

  SDL_RenderPresent(_renderer.get());
}
}  // namespace chip8