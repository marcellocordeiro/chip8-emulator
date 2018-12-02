#include "sound.h"

namespace chip8 {
sound::sound() {
  _buffer.loadFromFile("../audio/beep.wav");
  _sound.setBuffer(_buffer);

  _sound.setVolume(_sound.getVolume() * 0.1f);
  _sound.setLoop(false);
}

void sound::play() {
  _sound.play();
}

void sound::stop() {
  _sound.stop();
}
}  // namespace chip8