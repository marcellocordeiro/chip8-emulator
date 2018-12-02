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
  /* std::for_each(_gfx.begin(), _gfx.end(),
    [](auto&& arr) { arr.fill(sf::Color::Black); }
  ); */ // for documentation purposes

  _gfx.fill({});

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