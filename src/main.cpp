#include <SFML/Graphics.hpp>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include "cpu.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace cte {
// graphics
constexpr auto scale = 20.f;
constexpr auto res_scale = 20u;
constexpr auto fps_limit = 60u;

// audio
// todo

// cpu
constexpr auto cpu_clock = 840.f;
// constexpr auto cpu_counters_clock = 60.0f;
}  // namespace cte

void render(const std::array<bool, cte::gfx_size>& gfx, sf::RenderWindow& window) {
  window.clear();
  sf::RectangleShape rectangle(sf::Vector2f(cte::scale, cte::scale));

  for (int y = 0; y < cte::screen_height; ++y) {
    for (int x = 0; x < cte::screen_width; ++x) {
      if (gfx[x + (y * cte::screen_width)]) {
        rectangle.setPosition(x * cte::scale, y * cte::scale);
        window.draw(rectangle);
      }
    }
  }

  window.display();
}

void update_keys(std::array<bool, cte::keys_size>& keys) {
  keys[0] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1);
  keys[1] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2);
  keys[2] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3);
  keys[3] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4);

  keys[4] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
  keys[5] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
  keys[6] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
  keys[7] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R);

  keys[8] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
  keys[9] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
  keys[10] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
  keys[11] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);

  keys[12] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z);
  keys[13] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X);
  keys[14] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C);
  keys[15] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V);
}

int main() {
  cpu chip8;

  std::string rom_file = "BRIX";
  // auto rom_path = std::filesystem::current_path().parent_path() / "roms" / rom_file;
  chip8.load(rom_file);

#ifdef _WIN32
  ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

  sf::RenderWindow window(sf::VideoMode(cte::screen_width * cte::res_scale, cte::screen_height * cte::res_scale), "this shit doesn't work");
  window.setFramerateLimit(cte::fps_limit);
  window.clear();
  window.display();

  sf::Clock clock;
  float refresh_speed = 1.f / cte::cpu_clock;

  try {
    while (window.isOpen()) {
      sf::Event event;

      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
          window.close();
        }
      }

      if (clock.getElapsedTime().asSeconds() >= refresh_speed) {
        chip8.cycle();
        update_keys(chip8.get_keys());

        if (chip8.get_draw()) {
          render(chip8.get_gfx(), window);
        }

        clock.restart();
      }
    }
  } catch (std::exception e) {
    window.close();

#ifdef _WIN32
    MessageBox(NULL, e.what(), "Error!", MB_OK);
#endif
  }

  return 0;
}