#include "sound.h"

namespace chip8 {
sound::sound() {
  std::array<sf::Int16, cte::samples> wavetable;

  for (std::size_t i = 0; i < wavetable.size(); ++i) {
    wavetable[i] = static_cast<sf::Int16>(std::sin(i * cte::two_pi * cte::step) * cte::amplitude);
  }

  _buffer.loadFromSamples(wavetable.data(), wavetable.size(), 1, cte::sampling_rate);

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