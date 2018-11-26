#include "display.h"

namespace chip8 {
display::display() {
  _window.create(sf::VideoMode(cte::screen_width * cte::res_scale, cte::screen_height * cte::res_scale), "chip8-emulator");
  _window.setFramerateLimit(cte::fps_limit);
  _window.clear();
  _window.display();
}

bool display::is_open() {
  return _window.isOpen();
}

actions display::poll_events() {
  auto action = actions::NONE;
  sf::Event event;

  while (_window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      _window.close();
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
      _window.close();
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1) {
      action = actions::RESET;
    }
  }

  return action;
}

void display::render(const std::array<bool, cte::gfx_size>& gfx) {
  _window.clear();
  sf::RectangleShape rectangle(sf::Vector2f(cte::scale, cte::scale));

  for (int y = 0; y < cte::screen_height; ++y) {
    for (int x = 0; x < cte::screen_width; ++x) {
      if (gfx[x + (y * cte::screen_width)]) {
        rectangle.setPosition(x * cte::scale, y * cte::scale);
        _window.draw(rectangle);
      }
    }
  }

  _window.display();
}

void display::close() {
  _window.close();
}
}  // namespace chip8

/* display::display() {
  SDL_Init(SDL_INIT_VIDEO);

  _window = SDL_CreateWindow("chip8-emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, cte::screen_width * cte::res_scale, cte::screen_height * cte::res_scale, SDL_WINDOW_SHOWN);
  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
}

display::~display() {
  // SDL_DestroyRenderer(_renderer);
  // SDL_DestroyWindow(_window);
  // SDL_Quit();
}

void display::poll_events() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      close();
    }
  }
}

bool display::is_open() {
  return (_renderer != nullptr);
}

void display::close() {
  SDL_Quit();

  _window = nullptr;
  _renderer = nullptr;
}

void display::render(const std::array<bool, cte::gfx_size>& gfx) {
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
  SDL_RenderClear(_renderer);
  SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

  SDL_Rect rect;

  rect.x = 0;
  rect.y = 0;
  rect.h = cte::res_scale;
  rect.w = cte::res_scale;

  for (int y = 0; y < cte::screen_height; ++y) {
    for (int x = 0; x < cte::screen_width; ++x) {
      rect.x = x * cte::res_scale;
      rect.y = y * cte::res_scale;

      if (gfx[x + (y * cte::screen_width)]) {
        SDL_RenderFillRect(_renderer, &rect);
      }
    }
  }

  SDL_RenderPresent(_renderer);
} */