#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>

#include "common.hpp"

namespace chip8 {
class timer {
public:
  float elapsed_time();
  void restart();

private:
  sf::Clock _clock;
};
}