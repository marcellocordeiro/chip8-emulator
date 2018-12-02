#include "display.h"

namespace chip8 {
display::display() {
  _window.create(sf::VideoMode(ct::res_width, ct::res_height), "chip8-emulator");
  _window.setFramerateLimit(ct::fps_limit);
  _window.clear();
  _window.display();
}

bool display::is_open() {
  return _window.isOpen();
}

void display::close() {
  _window.close();
}

ac display::poll_events() {
  auto action = ac::none;
  sf::Event event;

  while (_window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      _window.close();
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
      action = ac::pause;
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1) {
      action = ac::reset;
    }
  }

  return action;
}

void display::clear() {
  for (auto& el : _gfx) {
    el.fill(sf::Color::Black);
  }

  render();
}

const sf::Color& display::get_pixel(std::size_t x, std::size_t y) const {
  return _gfx[x][y];
}

void display::set_pixel(std::size_t x, std::size_t y, const sf::Color& color) {
  _gfx[x][y] = color;
}

void display::render() {
  sf::RectangleShape rect(sf::Vector2f(ct::scale, ct::scale));
  _window.clear();

  for (std::size_t y = 0; y < ct::gfx_height; ++y) {
    for (std::size_t x = 0; x < ct::gfx_width; ++x) {
      rect.setFillColor(_gfx[x][y]);
      rect.setPosition(x * ct::scale, y * ct::scale);
      _window.draw(rect);
    }
  }

  _window.display();
}
}  // namespace chip8

/* display::display() {
  SDL_Init(SDL_INIT_VIDEO);

  _window = SDL_CreateWindow("chip8-emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ct::screen_width * ct::res_scale, ct::screen_height * ct::res_scale, SDL_WINDOW_SHOWN);
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

void display::render(const std::array<bool, ct::gfx_size>& gfx) {
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
  SDL_RenderClear(_renderer);
  SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

  SDL_Rect rect;

  rect.x = 0;
  rect.y = 0;
  rect.h = ct::res_scale;
  rect.w = ct::res_scale;

  for (int y = 0; y < ct::screen_height; ++y) {
    for (int x = 0; x < ct::screen_width; ++x) {
      rect.x = x * ct::res_scale;
      rect.y = y * ct::res_scale;

      if (gfx[x + (y * ct::screen_width)]) {
        SDL_RenderFillRect(_renderer, &rect);
      }
    }
  }

  SDL_RenderPresent(_renderer);
} */