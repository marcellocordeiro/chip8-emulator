#include "display.h"

namespace chip8 {
display::display()
{
  this->window.create(
      sf::VideoMode(ct::res_width, ct::res_height), "chip8-emulator");
  this->window.setFramerateLimit(ct::fps_limit);
  this->window.clear();
  this->window.display();
}

bool display::is_open()
{
  return this->window.isOpen();
}

void display::close()
{
  this->window.close();
}

std::queue<action_type>& display::poll_events()
{
  sf::Event event;

  while (this->window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      this->window.close();
    }

    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {
      actions.push(action_type::pause);
    }

    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::F1) {
      actions.push(action_type::reset);
    }
  }

  return actions;
}

void display::clear()
{
  /* std::for_each(this->gfx.begin(), this->gfx.end(),
    [](auto&& arr) { arr.fill(sf::Color::Black); }
  ); */ // for documentation purposes

  this->gfx.fill({});

  render();
}

std::uint32_t display::get_pixel(const size_t x, const size_t y) const
{
  return this->gfx[x][y].toInteger();
}

void display::set_pixel(
    const size_t x, const size_t y, const std::uint32_t color)
{
  this->gfx[x][y] = sf::Color(color);
}

void display::render()
{
  this->window.clear();

  for (size_t y = 0; y < ct::gfx_height; ++y) {
    for (size_t x = 0; x < ct::gfx_width; ++x) {
      sf::RectangleShape rect(sf::Vector2f(ct::scale, ct::scale));
      rect.setFillColor(this->gfx[x][y]);
      rect.setPosition(x * ct::scale, y * ct::scale);

      this->window.draw(rect);
    }
  }

  this->window.display();
}
}  // namespace chip8