#include "timer.h"

namespace chip8 {
float timer::elapsed_time()
{
  const auto end = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<float> elapsed = end - this->start;

  return elapsed.count();
}

void timer::restart()
{
  this->start = std::chrono::high_resolution_clock::now();
}
}  // namespace chip8
