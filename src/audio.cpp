#include "audio.h"

namespace chip8 {
audio::audio() {
  this->buffer.loadFromFile("../audio/beep.wav");
  this->sound.setBuffer(this->buffer);

  this->sound.setVolume(this->sound.getVolume() * 0.1f);
  this->sound.setLoop(false);
}

void audio::play() {
  this->sound.play();
}

void audio::stop() {
  this->sound.stop();
}
}  // namespace chip8