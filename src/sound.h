#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <vector>

#include "common.hpp"

namespace chip8 {
class sound {
public:
  sound();
  void play();
  void stop();

private:
  sf::SoundBuffer _buffer;
  sf::Sound _sound;
};
}