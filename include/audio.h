#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace chip8 {
class audio {
public:
  audio();
  void play();
  void stop();

private:
  sf::SoundBuffer buffer;
  sf::Sound       sound;
};
}  // namespace chip8