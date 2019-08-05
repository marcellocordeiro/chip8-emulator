#include "io.h"

#include "audio.h"
#include "cpu.h"
#include "emulator.h"
#include "input.h"
#include "system_utils.h"

namespace chip8 {
io::io(chip8::emulator& emulator_ref) : emulator(emulator_ref)
{
  window.create(sf::VideoMode(ct::res_width, ct::res_height), "chip8-emulator");
  window.setFramerateLimit(ct::fps_limit);
  window.clear();
  window.display();
}

io::~io()
{
  window.close();
}

void io::clear()
{
  /* std::for_each(frame_buffer.begin(), frame_buffer.end(),
    [](auto&& arr) { arr.fill(sf::Color::Black); }
  ); */ // for documentation purposes

  frame_buffer.fill(sf::Color::Black);

  draw();
}

uint32_t io::get_pixel(size_t x, size_t y) const
{
  return frame_buffer[x + y * ct::width].toInteger();
}

void io::set_pixel(size_t x, size_t y, uint32_t color)
{
  frame_buffer[x + y * ct::width] = sf::Color(color);
}

void io::draw()
{
  window.clear();

  for (size_t y = 0; y < ct::height; ++y) {
    for (size_t x = 0; x < ct::width; ++x) {
      sf::RectangleShape rect(sf::Vector2f(ct::scale, ct::scale));
      rect.setFillColor(frame_buffer[x + y * ct::width]);
      rect.setPosition(x * ct::scale, y * ct::scale);

      window.draw(rect);
    }
  }

  window.display();
}

void io::run()
{
  running = true;

  while (true) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        return;
      }

      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Escape) {
        running = !running;
      }

      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::F1) {
        emulator.get_cpu()->reset();
        emulator.get_cpu()->load(lib::select_rom());
      }
    }

    if (running) emulator.get_cpu()->cycle();
  }
}
}  // namespace chip8
