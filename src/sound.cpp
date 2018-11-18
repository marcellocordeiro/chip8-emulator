#include "sound.h"

namespace chip8 {
sound::sound() {
  std::vector<sf::Int16> wavetable;
  for (float i = 0.f; i < cte::stepCount; ++i) {
    wavetable.push_back(static_cast<sf::Int16>(std::sin(cte::step * i) * cte::amplitude));
  }

  // sf::SoundBuffer;
  _buffer.loadFromSamples(wavetable.data(), wavetable.size(), 1u, static_cast<sf::Uint32>(cte::updateRate));

  // sf::Sound;
  _sound.setBuffer(_buffer);
  _sound.setLoop(true);

  _sound.setVolume(_sound.getVolume() * 0.5);
}

void sound::play() {
  _sound.play();
}

void sound::stop() {
  _sound.stop();
}
}  // namespace chip8