#include "timer.h"

namespace chip8 {
float timer::elapsed_time() {
  return _clock.getElapsedTime().asSeconds();
}

void timer::restart() {
  _clock.restart();
}
}  // namespace chip8